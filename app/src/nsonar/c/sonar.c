#include "sonar.h"
#include <stdint.h>
#include <unistd.h>
#include <fcntl.h>
#include <linux/i2c-dev.h>
//#include <i2c/smbus.h>

struct Pointer
{
    int fd;
    uint8_t addr;
    uint8_t req;
};

long init(int adapter_nr)
{
    Pointer* p = new Pointer();
    char filename[20];
    snprintf(filename, 19, "/dev/i2c-%d", adapter_nr);
    p->fd = open(filename, O_RDWR);
    p->addr = MB12XX_ADDRESS;
    p->req = MB12XX_WRITE_BYTE;
    return (long) p;
}

JNIEXPORT jlong JNICALL Java_lv_kaneps_rpidrone_drivers_SonarSensorDriver_init(JNIEnv* env, jobject obj, jint adapter)
{
  return (jlong) init(adapter);
}

JNIEXPORT void JNICALL Java_lv_kaneps_rpidrone_drivers_SonarSensorDriver_release(JNIEnv* env, jobject obj, jlong pLong)
{
  Pointer* p = (Pointer*) pLong;
  close(p->fd);
  delete p;
}

JNIEXPORT jfloat JNICALL Java_lv_kaneps_rpidrone_drivers_SonarSensorDriver_getMinAltitude(JNIEnv*, jobject, jlong pLong)
{
  return 0.02f;
}

JNIEXPORT jfloat JNICALL Java_lv_kaneps_rpidrone_drivers_SonarSensorDriver_getMaxAltitude(JNIEnv*, jobject, jlong pLong)
{
  return 7.0f;
}

JNIEXPORT jfloat JNICALL Java_lv_kaneps_rpidrone_drivers_SonarSensorDriver_getAltitude(JNIEnv*, jobject, jlong pLong)
{
  Pointer* p = (Pointer*) pLong;

  uint8_t buffer[2] = {0, 0};
  if(ioctl(p->fd, I2C_SLAVE, p->addr) < 0) return -1;
  if(i2c_smbus_write_byte(p->fd, p->req) == -1) return -2;
  if(read(p->fd, &buffer[0], 2) != 2) return -3;

  float alt = (float) ((((int) buffer[0]) << 8) | buffer[1]) / 100.0f;

  return alt;
}