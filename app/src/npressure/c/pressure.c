#define pointer (Pointer*)

#include "pressure.h"
#include <cmath>
#include <stdint.h>
#include <unistd.h>
#include <fcntl.h>
#include <linux/i2c-dev.h>
//#include <i2c/smbus.h>

static int adapter_nr = 1; // hardcoded i2c bus number - works on raspberry pi 3 model b

typedef enum
{
    BMP085_MODE_ULTRALOWPOWER = 0,
    BMP085_MODE_STANDARD      = 1,
    BMP085_MODE_HIGHRES       = 2,
    BMP085_MODE_ULTRAHIGHRES  = 3
} bmp085_mode_t;

typedef struct
{
    int16_t  ac1;
    int16_t  ac2;
    int16_t  ac3;
    uint16_t ac4;
    uint16_t ac5;
    uint16_t ac6;
    int16_t  b1;
    int16_t  b2;
    int16_t  mb;
    int16_t  mc;
    int16_t  md;
} bmp085_calib_data;

struct Pointer
{
    int fd;
    bmp085_calib_data _bmp085_coeffs;
    uint8_t _bmp085Mode;
};

int8_t writeCommand(int fd, uint8_t reg, uint8_t value)
{
    if(ioctl(fd, I2C_SLAVE, BMP085_ADDRESS) < 0) return -1;
    if(i2c_smbus_write_byte_data(fd, reg, value) != 0) return -2;
    return 0;
}

int8_t read8(int fd, uint8_t reg, uint8_t* value)
{
    if(ioctl(fd, I2C_SLAVE, BMP085_ADDRESS) < 0) return -1;
    uint8_t rega[1] = {reg};
    uint8_t buf[1] = {0};
    if(write(fd, &rega[0], 1) != 1) return -2;
    if(read(fd, &buf[0], 1) != 1) return -3;
    *value = buf[0];
    return 0;
}

int8_t read16(int fd, uint8_t reg, uint16_t* value)
{
    if(ioctl(fd, I2C_SLAVE, BMP085_ADDRESS) < 0) return -1;
    uint8_t rega[1] = {reg};
    uint8_t buf[2] = {0, 0};
    if(write(fd, &rega[0], 1) != 1) return -2;
    if(read(fd, &buf[0], 2) != 2) return -3;
    *value = ((uint16_t) buf[0]) << 8 | buf[1];
    return 0;
}

int8_t readS16(int fd, uint8_t reg, int16_t* value)
{
    uint16_t uval;
    int8_t ret;
    if((ret = read16(fd, reg, &uval)) >= 0) *value = (int16_t) uval;
    return ret;
}

int32_t computeB5(Pointer* p, int32_t ut)
{
    int32_t X1 = (ut - (int32_t)(p->_bmp085_coeffs.ac6)) * ((int32_t)(p->_bmp085_coeffs.ac5)) >> 15;
    int32_t X2 = ((int32_t)(p->_bmp085_coeffs.mc << 11)) / (X1+(int32_t)(p->_bmp085_coeffs.md));
    return X1 + X2;
}

void readCoefficients(Pointer* p)
{
    readS16(p->fd, BMP085_REGISTER_CAL_AC1, &(p->_bmp085_coeffs.ac1));
    readS16(p->fd, BMP085_REGISTER_CAL_AC2, &(p->_bmp085_coeffs.ac2));
    readS16(p->fd, BMP085_REGISTER_CAL_AC3, &(p->_bmp085_coeffs.ac3));
    read16(p->fd, BMP085_REGISTER_CAL_AC4, &(p->_bmp085_coeffs.ac4));
    read16(p->fd, BMP085_REGISTER_CAL_AC5, &(p->_bmp085_coeffs.ac5));
    read16(p->fd, BMP085_REGISTER_CAL_AC6, &(p->_bmp085_coeffs.ac6));
    readS16(p->fd, BMP085_REGISTER_CAL_B1, &(p->_bmp085_coeffs.b1));
    readS16(p->fd, BMP085_REGISTER_CAL_B2, &(p->_bmp085_coeffs.b2));
    readS16(p->fd, BMP085_REGISTER_CAL_MB, &(p->_bmp085_coeffs.mb));
    readS16(p->fd, BMP085_REGISTER_CAL_MC, &(p->_bmp085_coeffs.mc));
    readS16(p->fd, BMP085_REGISTER_CAL_MD, &(p->_bmp085_coeffs.md));
}

long init(Pointer* p, bmp085_mode_t mode)
{
    p = new Pointer();

    if((mode > BMP085_MODE_ULTRAHIGHRES) || (mode < 0))
      mode = BMP085_MODE_ULTRAHIGHRES;

    char filename[20];
    snprintf(filename, 19, "/dev/i2c-%d", adapter_nr);
    p->fd = open(filename, O_RDWR);

    uint8_t id;
    read8(p->fd, BMP085_REGISTER_CHIPID, &id);
    if(id != 0x55)
    {
        delete p;
        return 0L;
    }

    p->_bmp085Mode = mode;

    readCoefficients(p);

    return (long) p;
}

int readRawPressure(Pointer* p, int32_t* pressure)
{
    uint8_t  p8;
    uint16_t p16;
    int32_t  p32;

    writeCommand(p->fd, BMP085_REGISTER_CONTROL, BMP085_REGISTER_READPRESSURECMD + ((p->_bmp085Mode) << 6));
    switch(p->_bmp085Mode)
    {
      case BMP085_MODE_ULTRALOWPOWER:
        usleep(5000);
        break;
      case BMP085_MODE_STANDARD:
        usleep(8000);
        break;
      case BMP085_MODE_HIGHRES:
        usleep(14000);
        break;
      case BMP085_MODE_ULTRAHIGHRES:
      default:
        usleep(26000);
        break;
    }

    read16(p->fd, BMP085_REGISTER_PRESSUREDATA, &p16);
    p32 = (uint32_t)p16 << 8;
    read8(p->fd, BMP085_REGISTER_PRESSUREDATA+2, &p8);
    p32 += p8;
    p32 >>= (8 - (p->_bmp085Mode));

    *pressure = p32;
}

void readRawTemp(Pointer* p, int32_t* temp)
{
    uint16_t t;
    writeCommand(p->fd, BMP085_REGISTER_CONTROL, BMP085_REGISTER_READTEMPCMD);
    usleep(5000);
    read16(p->fd, BMP085_REGISTER_TEMPDATA, &t);
    *temp = t;
}

/**************************************************************************/
/*!
    @brief  Gets the compensated pressure level in kPa
*/
/**************************************************************************/
float getPressure(Pointer* po)
{
    int32_t  ut = 0, up = 0, compp = 0;
    int32_t  x1, x2, b5, b6, x3, b3, p;
    uint32_t b4, b7;

    /* Get the raw pressure and temperature values */
    readRawTemp(po, &ut);
    readRawPressure(po, &up);

    /* Temperature compensation */
    b5 = computeB5(po, ut);

    /* Pressure compensation */
    b6 = b5 - 4000;
    x1 = ((po->_bmp085_coeffs.b2) * ((b6 * b6) >> 12)) >> 11;
    x2 = ((po->_bmp085_coeffs.ac2) * b6) >> 11;
    x3 = x1 + x2;
    b3 = (((((int32_t) (po->_bmp085_coeffs.ac1)) * 4 + x3) << (po->_bmp085Mode)) + 2) >> 2;
    x1 = ((po->_bmp085_coeffs.ac3) * b6) >> 13;
    x2 = ((po->_bmp085_coeffs.b1) * ((b6 * b6) >> 12)) >> 16;
    x3 = ((x1 + x2) + 2) >> 2;
    b4 = ((po->_bmp085_coeffs.ac4) * (uint32_t) (x3 + 32768)) >> 15;
    b7 = ((uint32_t) (up - b3) * (50000 >> (po->_bmp085Mode)));

    if (b7 < 0x80000000)
      p = (b7 << 1) / b4;
    else
      p = (b7 / b4) << 1;

    x1 = (p >> 8) * (p >> 8);
    x1 = (x1 * 3038) >> 16;
    x2 = (-7357 * p) >> 16;
    compp = p + ((x1 + x2 + 3791) >> 4);

    /* Return compensated pressure value */
    return compp;
}

/**************************************************************************/
/*!
    @brief  Reads the temperatures in degrees Celsius
*/
/**************************************************************************/
float getTemperature(Pointer* p)
{
    int32_t UT, B5; // following ds convention
    float t;

    readRawTemp(p, &UT);

    B5 = computeB5(p, UT);
    t = (B5+8) >> 4;
    t /= 10;

    return t;
}

/**************************************************************************/
/*!
    Calculates the altitude (in meters) from the specified atmospheric
    pressure (in hPa), and sea-level pressure (in hPa).
    @param  seaLevel      Sea-level pressure in hPa
    @param  atmospheric   Atmospheric pressure in hPa
*/
/**************************************************************************/
static float pressureToAltitude(float seaLevel, float atmospheric)
{
  // Equation taken from BMP180 datasheet (page 16):
  //  http://www.adafruit.com/datasheets/BST-BMP180-DS000-09.pdf

  // Note that using the equation from wikipedia can give bad results
  // at high altitude.  See this thread for more information:
  //  http://forums.adafruit.com/viewtopic.php?f=22&t=58064

  return 44330.0 * (1.0 - std::pow(atmospheric / seaLevel, 0.1903));
}

/**************************************************************************/
/*!
    Calculates the altitude (in meters) from the specified atmospheric
    pressure (in hPa), and sea-level pressure (in hPa).  Note that this
    function just calls the overload of pressureToAltitude which takes
    seaLevel and atmospheric pressure--temperature is ignored.  The original
    implementation of this function was based on calculations from Wikipedia
    which are not accurate at higher altitudes.  To keep compatibility with
    old code this function remains with the same interface, but it calls the
    more accurate calculation.
    @param  seaLevel      Sea-level pressure in hPa
    @param  atmospheric   Atmospheric pressure in hPa
    @param  temp          Temperature in degrees Celsius
*/
/**************************************************************************/
static float pressureToAltitude(float seaLevel, float atmospheric, float temp)
{
  return pressureToAltitude(seaLevel, atmospheric);
}

/**************************************************************************/
/*!
    Calculates the pressure at sea level (in hPa) from the specified altitude
    (in meters), and atmospheric pressure (in hPa).
    @param  altitude      Altitude in meters
    @param  atmospheric   Atmospheric pressure in hPa
*/
/**************************************************************************/
static float seaLevelForAltitude(float altitude, float atmospheric)
{
  // Equation taken from BMP180 datasheet (page 17):
  //  http://www.adafruit.com/datasheets/BST-BMP180-DS000-09.pdf

  // Note that using the equation from wikipedia can give bad results
  // at high altitude.  See this thread for more information:
  //  http://forums.adafruit.com/viewtopic.php?f=22&t=58064

  return atmospheric / std::pow(1.0 - (altitude/44330.0), 5.255);
}

/**************************************************************************/
/*!
    Calculates the pressure at sea level (in hPa) from the specified altitude
    (in meters), and atmospheric pressure (in hPa).  Note that this
    function just calls the overload of seaLevelForAltitude which takes
    altitude and atmospheric pressure--temperature is ignored.  The original
    implementation of this function was based on calculations from Wikipedia
    which are not accurate at higher altitudes.  To keep compatibility with
    old code this function remains with the same interface, but it calls the
    more accurate calculation.
    @param  altitude      Altitude in meters
    @param  atmospheric   Atmospheric pressure in hPa
    @param  temp          Temperature in degrees Celsius
*/
/**************************************************************************/
static float seaLevelForAltitude(float altitude, float atmospheric, float temp)
{
  return seaLevelForAltitude(altitude, atmospheric);
}

#define ABS_ZERO_C (-273.15f)

JNIEXPORT jlong JNICALL Java_lv_kaneps_rpidrone_drivers_PressureSensorDriver_init(JNIEnv* env, jobject obj, jint mode)
{
  Pointer* p = new Pointer();

  return (jlong) init(p, (bmp085_mode_t) mode);
}

JNIEXPORT void JNICALL Java_lv_kaneps_rpidrone_drivers_PressureSensorDriver_release(JNIEnv* env, jobject obj, jlong pLong)
{
  if(pLong == 0) return;
  Pointer* p = pointer pLong;
  close(p->fd);
  delete p;
}

JNIEXPORT jfloat JNICALL Java_lv_kaneps_rpidrone_drivers_PressureSensorDriver_getPressure(JNIEnv* env, jobject obj, jlong pLong)
{
  if(pLong == 0) return 0;
  Pointer* p = pointer pLong;

  return (jfloat) getPressure(p);
}

JNIEXPORT jfloat JNICALL Java_lv_kaneps_rpidrone_drivers_PressureSensorDriver_getTemperature(JNIEnv* env, jobject obj, jlong pLong)
{
  if(pLong == 0) return ABS_ZERO_C;
  Pointer* p = pointer pLong;

  return (jfloat) getTemperature(p);
}

JNIEXPORT jfloat JNICALL Java_lv_kaneps_rpidrone_drivers_PressureSensorDriver_getTemperatureK(JNIEnv* env, jobject obj, jlong pLong)
{
  if(pLong == 0) return 0.0f;
  Pointer* p = pointer pLong;

  return (jfloat) (getTemperature(p) + (-ABS_ZERO_C));
}

JNIEXPORT jfloat JNICALL Java_lv_kaneps_rpidrone_drivers_PressureSensorDriver_getAltitude(JNIEnv* env, jobject obj, jlong pLong)
{
  if(pLong == 0 || seaLevel == 0) return 0;
	Pointer* p = pointer pLong;

	return (jfloat) pressureToAltitude(seaLevel, getPressure(p));//(44330.0d * (1.0d - (std::pow((double) data.pressure / (double) seaLevel, toPow))));
}