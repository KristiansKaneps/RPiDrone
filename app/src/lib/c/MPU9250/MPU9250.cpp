/*
   MPU9250.h : Header for Raspberry Pi MPU9250 library

   Copyright (c) Simon D. Levy 2016

   Adapted from https://github.com/bolderflight/MPU9250/blob/master/MPU9250.h

   Permission is hereby granted, free of charge, to any person obtaining a copy of this software 
   and associated documentation files (the "Software"), to deal in the Software without restriction, 
   including without limitation the rights to use, copy, modify, merge, publish, distribute, 
   sublicense, and/or sell copies of the Software, and to permit persons to whom the Software is 
   furnished to do so, subject to the following conditions:

   The above copyright notice and this permission notice shall be included in all copies or 
   substantial portions of the Software.

   THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING 
   BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND 
   NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, 
   DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, 
   OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
 */

#include "MPU9250.h"

#include <string.h>
#include <cmath>
#include <stdint.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/ioctl.h>
#include <asm/ioctl.h>
#include <linux/spi/spidev.h>

#include <stdio.h>
#include <stdlib.h>

/* MPU9250 object, input the SPI bus and speed */
MPU9250::MPU9250(uint8_t bus, uint32_t speed){
    _spi_bus = bus;
    _spi_speed = speed;
}

/* starts I2C communication and sets up the MPU-9250 */
int MPU9250::begin(mpu9250_accel_range accelRange, mpu9250_gyro_range gyroRange){
    uint8_t buff[3];
    uint8_t data[7];

    if (SPISetup(_spi_bus, _spi_speed) < 0) 
        return -1;

	// reset the MPU9250
	writeRegister(PWR_MGMNT_1,PWR_RESET);

    // wait for oscillators to stabilize
    usleep(20000);

    // select clock source to gyro
    if( !writeRegister(PWR_MGMNT_1,CLOCK_SEL_PLL) ){
        return -1;
    }

    // check the WHO AM I byte, expected value is 0x71 (decimal 113)
    if( whoAmI() != 113 ){
        return -1;
    }

    // enable accelerometer and gyro
    if( !writeRegister(PWR_MGMNT_2,SEN_ENABLE) ){
        return -1;
    }

    /* setup the accel and gyro ranges */
    switch(accelRange) {

        case ACCEL_RANGE_2G:
            // setting the accel range to 2G
            if( !writeRegister(ACCEL_CONFIG,ACCEL_FS_SEL_2G) ){
                return -1;
            }
            _accelScale = G * 2.0f/32767.5f; // setting the accel scale to 2G
            break;

        case ACCEL_RANGE_4G:
            // setting the accel range to 4G
            if( !writeRegister(ACCEL_CONFIG,ACCEL_FS_SEL_4G) ){
                return -1;
            }
            _accelScale = G * 4.0f/32767.5f; // setting the accel scale to 4G
            break;

        case ACCEL_RANGE_8G:
            // setting the accel range to 8G
            if( !writeRegister(ACCEL_CONFIG,ACCEL_FS_SEL_8G) ){
                return -1;
            }
            _accelScale = G * 8.0f/32767.5f; // setting the accel scale to 8G
            break;

        case ACCEL_RANGE_16G:
            // setting the accel range to 16G
            if( !writeRegister(ACCEL_CONFIG,ACCEL_FS_SEL_16G) ){
                return -1;
            }
            _accelScale = G * 16.0f/32767.5f; // setting the accel scale to 16G
            break;
    }

    switch(gyroRange) {
        case GYRO_RANGE_250DPS:
            // setting the gyro range to 250DPS
            if( !writeRegister(GYRO_CONFIG,GYRO_FS_SEL_250DPS) ){
                return -1;
            }
            _gyroScale = 250.0f/32767.5f * _d2r; // setting the gyro scale to 250DPS
            break;

        case GYRO_RANGE_500DPS:
            // setting the gyro range to 500DPS
            if( !writeRegister(GYRO_CONFIG,GYRO_FS_SEL_500DPS) ){
                return -1;
            }
            _gyroScale = 500.0f/32767.5f * _d2r; // setting the gyro scale to 500DPS
            break;

        case GYRO_RANGE_1000DPS:
            // setting the gyro range to 1000DPS
            if( !writeRegister(GYRO_CONFIG,GYRO_FS_SEL_1000DPS) ){
                return -1;
            }
            _gyroScale = 1000.0f/32767.5f * _d2r; // setting the gyro scale to 1000DPS
            break;

        case GYRO_RANGE_2000DPS:
            // setting the gyro range to 2000DPS
            if( !writeRegister(GYRO_CONFIG,GYRO_FS_SEL_2000DPS) ){
                return -1;
            }
            _gyroScale = 2000.0f/32767.5f * _d2r; // setting the gyro scale to 2000DPS
            break;
    }

    // enable I2C master mode
    if( !writeRegister(USER_CTRL,I2C_MST_EN) ){
    	return -1;
    }

	// set the I2C bus speed to 400 kHz
	if( !writeRegister(I2C_MST_CTRL,I2C_MST_CLK) ){
		return -1;
	}

	// check AK8963 WHO AM I register, expected value is 0x48 (decimal 72)
	if( whoAmIAK8963() != 72 ){
        return -1;
	}

    /* get the magnetometer calibration */

    // set AK8963 to Power Down
    if( !writeAK8963Register(AK8963_CNTL1,AK8963_PWR_DOWN) ){
        return -1;
    }
    usleep(100000); // long wait between AK8963 mode changes

    // set AK8963 to FUSE ROM access
    if( !writeAK8963Register(AK8963_CNTL1,AK8963_FUSE_ROM) ){
        return -1;
    }
    usleep(100000); // long wait between AK8963 mode changes

    // read the AK8963 ASA registers and compute magnetometer scale factors
    readAK8963Registers(AK8963_ASA,sizeof(buff),&buff[0]);
    _magScaleX = ((((float)buff[0]) - 128.0f)/(256.0f) + 1.0f) * 4912.0f / 32760.0f; // micro Tesla
    _magScaleY = ((((float)buff[1]) - 128.0f)/(256.0f) + 1.0f) * 4912.0f / 32760.0f; // micro Tesla
    _magScaleZ = ((((float)buff[2]) - 128.0f)/(256.0f) + 1.0f) * 4912.0f / 32760.0f; // micro Tesla 

    // set AK8963 to Power Down
    if( !writeAK8963Register(AK8963_CNTL1,AK8963_PWR_DOWN) ){
        return -1;
    }
    usleep(100000); // long wait between AK8963 mode changes  

    // set AK8963 to 16 bit resolution, 100 Hz update rate
    if( !writeAK8963Register(AK8963_CNTL1,AK8963_CNT_MEAS2) ){
        return -1;
    }
    usleep(100000); // long wait between AK8963 mode changes     

    // instruct the MPU9250 to get 7 bytes of data from the AK8963 at the sample rate
    readAK8963Registers(AK8963_HXL,sizeof(data),&data[0]);

    // successful init, return 0
    return 0;
}


/* sets the DLPF and interrupt settings */
int MPU9250::setFilt(mpu9250_dlpf_bandwidth bandwidth, uint8_t SRD){
    uint8_t data[7];

    switch(bandwidth) {
        case DLPF_BANDWIDTH_184HZ:
            if( !writeRegister(ACCEL_CONFIG2,ACCEL_DLPF_184) ){ // setting accel bandwidth to 184Hz
                return -1;
            } 
            if( !writeRegister(CONFIG,GYRO_DLPF_184) ){ // setting gyro bandwidth to 184Hz
                return -1;
            }
            break;

        case DLPF_BANDWIDTH_92HZ:
            if( !writeRegister(ACCEL_CONFIG2,ACCEL_DLPF_92) ){ // setting accel bandwidth to 92Hz
                return -1;
            } 
            if( !writeRegister(CONFIG,GYRO_DLPF_92) ){ // setting gyro bandwidth to 92Hz
                return -1;
            }
            break; 

        case DLPF_BANDWIDTH_41HZ:
            if( !writeRegister(ACCEL_CONFIG2,ACCEL_DLPF_41) ){ // setting accel bandwidth to 41Hz
                return -1;
            } 
            if( !writeRegister(CONFIG,GYRO_DLPF_41) ){ // setting gyro bandwidth to 41Hz
                return -1;
            } 
            break;

        case DLPF_BANDWIDTH_20HZ:
            if( !writeRegister(ACCEL_CONFIG2,ACCEL_DLPF_20) ){ // setting accel bandwidth to 20Hz
                return -1;
            } 
            if( !writeRegister(CONFIG,GYRO_DLPF_20) ){ // setting gyro bandwidth to 20Hz
                return -1;
            }
            break;

        case DLPF_BANDWIDTH_10HZ:
            if( !writeRegister(ACCEL_CONFIG2,ACCEL_DLPF_10) ){ // setting accel bandwidth to 10Hz
                return -1;
            } 
            if( !writeRegister(CONFIG,GYRO_DLPF_10) ){ // setting gyro bandwidth to 10Hz
                return -1;
            }
            break;

        case DLPF_BANDWIDTH_5HZ:
            if( !writeRegister(ACCEL_CONFIG2,ACCEL_DLPF_5) ){ // setting accel bandwidth to 5Hz
                return -1;
            } 
            if( !writeRegister(CONFIG,GYRO_DLPF_5) ){ // setting gyro bandwidth to 5Hz
                return -1;
            }
            break; 
    }

    /* setting the sample rate divider */
    if( !writeRegister(SMPDIV,SRD) ){ // setting the sample rate divider
        return -1;
    } 

    if(SRD > 9){

        // set AK8963 to Power Down
        if( !writeAK8963Register(AK8963_CNTL1,AK8963_PWR_DOWN) ){
            return -1;
        }
        usleep(100000); // long wait between AK8963 mode changes  

        // set AK8963 to 16 bit resolution, 8 Hz update rate
        if( !writeAK8963Register(AK8963_CNTL1,AK8963_CNT_MEAS1) ){
            return -1;
        }
        usleep(100000); // long wait between AK8963 mode changes     

        // instruct the MPU9250 to get 7 bytes of data from the AK8963 at the sample rate
        readAK8963Registers(AK8963_HXL,sizeof(data),&data[0]);
    }

    /* setting the interrupt */
    if( !writeRegister(INT_PIN_CFG,INT_PULSE_50US) ){ // setup interrupt, 50 us pulse
        return -1;
    }  
    if( !writeRegister(INT_ENABLE,INT_RAW_RDY_EN) ){ // set to data ready
        return -1;
    }  

    // successful filter setup, return 0
    return 0; 
}

/* get accelerometer data given pointers to store the three values, return data as counts */
void MPU9250::getAccelCounts(int16_t* ax, int16_t* ay, int16_t* az){
    uint8_t buff[6];
    int16_t axx, ayy, azz;

    readRegisters(ACCEL_OUT, sizeof(buff), &buff[0]); // grab the data from the MPU9250

    axx = (((int16_t)buff[0]) << 8) | buff[1];  // combine into 16 bit values
    ayy = (((int16_t)buff[2]) << 8) | buff[3];
    azz = (((int16_t)buff[4]) << 8) | buff[5];

    *ax = tX[0]*axx + tX[1]*ayy + tX[2]*azz; // transform axes
    *ay = tY[0]*axx + tY[1]*ayy + tY[2]*azz;
    *az = tZ[0]*axx + tZ[1]*ayy + tZ[2]*azz;
}

/* get accelerometer data given pointers to store the three values */
void MPU9250::getAccel(float* ax, float* ay, float* az){
    int16_t accel[3];

    getAccelCounts(&accel[0], &accel[1], &accel[2]);

    *ax = ((float) accel[0]) * _accelScale; // typecast and scale to values
    *ay = ((float) accel[1]) * _accelScale;
    *az = ((float) accel[2]) * _accelScale;
}

/* get gyro data given pointers to store the three values, return data as counts */
void MPU9250::getGyroCounts(int16_t* gx, int16_t* gy, int16_t* gz){
    uint8_t buff[6];
    int16_t gxx, gyy, gzz;

    readRegisters(GYRO_OUT, sizeof(buff), &buff[0]); // grab the data from the MPU9250

    gxx = (((int16_t)buff[0]) << 8) | buff[1];  // combine into 16 bit values
    gyy = (((int16_t)buff[2]) << 8) | buff[3];
    gzz = (((int16_t)buff[4]) << 8) | buff[5];

    *gx = tX[0]*gxx + tX[1]*gyy + tX[2]*gzz; // transform axes
    *gy = tY[0]*gxx + tY[1]*gyy + tY[2]*gzz;
    *gz = tZ[0]*gxx + tZ[1]*gyy + tZ[2]*gzz;
}

/* get gyro data given pointers to store the three values */
void MPU9250::getGyro(float* gx, float* gy, float* gz){
    int16_t gyro[3];

    getGyroCounts(&gyro[0], &gyro[1], &gyro[2]);

    *gx = ((float) gyro[0]) * _gyroScale; // typecast and scale to values
    *gy = ((float) gyro[1]) * _gyroScale;
    *gz = ((float) gyro[2]) * _gyroScale;
}

/* get magnetometer data given pointers to store the three values, return data as counts */
void MPU9250::getMagCounts(int16_t* hx, int16_t* hy, int16_t* hz){
    uint8_t buff[7];

    // read the magnetometer data off the external sensor buffer
    readRegisters(EXT_SENS_DATA_00,sizeof(buff),&buff[0]);

    if( buff[6] == 0x10 ) { // check for overflow
        *hx = (((int16_t)buff[1]) << 8) | buff[0];  // combine into 16 bit values
        *hy = (((int16_t)buff[3]) << 8) | buff[2];
        *hz = (((int16_t)buff[5]) << 8) | buff[4];
    }
    else{
        *hx = 0;  
        *hy = 0;
        *hz = 0;
    }
}

/* get magnetometer data given pointers to store the three values */
void MPU9250::getMag(float* hx, float* hy, float* hz){
    int16_t mag[3];

    getMagCounts(&mag[0], &mag[1], &mag[2]);

    *hx = ((float) mag[0]) * _magScaleX; // typecast and scale to values
    *hy = ((float) mag[1]) * _magScaleY;
    *hz = ((float) mag[2]) * _magScaleZ;
}

/* get temperature data given pointer to store the value, return data as counts */
void MPU9250::getTempCounts(int16_t* t){
    uint8_t buff[2];

    readRegisters(TEMP_OUT, sizeof(buff), &buff[0]); // grab the data from the MPU9250

    *t = (((int16_t)buff[0]) << 8) | buff[1];  // combine into 16 bit value and return
}

/* get temperature data given pointer to store the values */
void MPU9250::getTemp(float* t){
    int16_t tempCount;

    getTempCounts(&tempCount);

    *t = (( ((float) tempCount) - _tempOffset )/_tempScale) + _tempOffset;
}

/* get accelerometer and gyro data given pointers to store values, return data as counts */
void MPU9250::getMotion6Counts(int16_t* ax, int16_t* ay, int16_t* az, int16_t* gx, int16_t* gy, int16_t* gz){
    uint8_t buff[14];
    int16_t axx, ayy, azz, gxx, gyy, gzz;

    readRegisters(ACCEL_OUT, sizeof(buff), &buff[0]); // grab the data from the MPU9250

    axx = (((int16_t)buff[0]) << 8) | buff[1];  // combine into 16 bit values
    ayy = (((int16_t)buff[2]) << 8) | buff[3];
    azz = (((int16_t)buff[4]) << 8) | buff[5];

    gxx = (((int16_t)buff[8]) << 8) | buff[9];
    gyy = (((int16_t)buff[10]) << 8) | buff[11];
    gzz = (((int16_t)buff[12]) << 8) | buff[13];

    *ax = tX[0]*axx + tX[1]*ayy + tX[2]*azz; // transform axes
    *ay = tY[0]*axx + tY[1]*ayy + tY[2]*azz;
    *az = tZ[0]*axx + tZ[1]*ayy + tZ[2]*azz;

    *gx = tX[0]*gxx + tX[1]*gyy + tX[2]*gzz;
    *gy = tY[0]*gxx + tY[1]*gyy + tY[2]*gzz;
    *gz = tZ[0]*gxx + tZ[1]*gyy + tZ[2]*gzz;
}

/* get accelerometer and gyro data given pointers to store values */
void MPU9250::getMotion6(float* ax, float* ay, float* az, float* gx, float* gy, float* gz){
    int16_t accel[3];
    int16_t gyro[3];

    getMotion6Counts(&accel[0], &accel[1], &accel[2], &gyro[0], &gyro[1], &gyro[2]);

    *ax = ((float) accel[0]) * _accelScale; // typecast and scale to values
    *ay = ((float) accel[1]) * _accelScale;
    *az = ((float) accel[2]) * _accelScale;

    *gx = ((float) gyro[0]) * _gyroScale;
    *gy = ((float) gyro[1]) * _gyroScale;
    *gz = ((float) gyro[2]) * _gyroScale;
}

/* get accelerometer, gyro and temperature data given pointers to store values, return data as counts */
void MPU9250::getMotion7Counts(int16_t* ax, int16_t* ay, int16_t* az, int16_t* gx, int16_t* gy, int16_t* gz, int16_t* t){
    uint8_t buff[14];
    int16_t axx, ayy, azz, gxx, gyy, gzz;

    readRegisters(ACCEL_OUT, sizeof(buff), &buff[0]); // grab the data from the MPU9250

    axx = (((int16_t)buff[0]) << 8) | buff[1];  // combine into 16 bit values
    ayy = (((int16_t)buff[2]) << 8) | buff[3];
    azz = (((int16_t)buff[4]) << 8) | buff[5];

    *t = (((int16_t)buff[6]) << 8) | buff[7];

    gxx = (((int16_t)buff[8]) << 8) | buff[9];
    gyy = (((int16_t)buff[10]) << 8) | buff[11];
    gzz = (((int16_t)buff[12]) << 8) | buff[13];

    *ax = tX[0]*axx + tX[1]*ayy + tX[2]*azz; // transform axes
    *ay = tY[0]*axx + tY[1]*ayy + tY[2]*azz;
    *az = tZ[0]*axx + tZ[1]*ayy + tZ[2]*azz;

    *gx = tX[0]*gxx + tX[1]*gyy + tX[2]*gzz;
    *gy = tY[0]*gxx + tY[1]*gyy + tY[2]*gzz;
    *gz = tZ[0]*gxx + tZ[1]*gyy + tZ[2]*gzz;
}

/* get accelerometer, gyro, and temperature data given pointers to store values */
void MPU9250::getMotion7(float* ax, float* ay, float* az, float* gx, float* gy, float* gz, float* t){
    int16_t accel[3];
    int16_t gyro[3];
    int16_t tempCount;

    getMotion7Counts(&accel[0], &accel[1], &accel[2], &gyro[0], &gyro[1], &gyro[2], &tempCount);

    *ax = ((float) accel[0]) * _accelScale; // typecast and scale to values
    *ay = ((float) accel[1]) * _accelScale;
    *az = ((float) accel[2]) * _accelScale;

    *gx = ((float) gyro[0]) * _gyroScale;
    *gy = ((float) gyro[1]) * _gyroScale;
    *gz = ((float) gyro[2]) * _gyroScale;

    *t = (( ((float) tempCount) - _tempOffset )/_tempScale) + _tempOffset; 
}

/* get accelerometer, gyro and magnetometer data given pointers to store values, return data as counts */
void MPU9250::getMotion9Counts(int16_t* ax, int16_t* ay, int16_t* az, int16_t* gx, int16_t* gy, int16_t* gz, int16_t* hx, int16_t* hy, int16_t* hz){
    uint8_t buff[21];
    int16_t axx, ayy, azz, gxx, gyy, gzz;

    readRegisters(ACCEL_OUT, sizeof(buff), &buff[0]); // grab the data from the MPU9250

    axx = (((int16_t)buff[0]) << 8) | buff[1];  // combine into 16 bit values
    ayy = (((int16_t)buff[2]) << 8) | buff[3];
    azz = (((int16_t)buff[4]) << 8) | buff[5];

    gxx = (((int16_t)buff[8]) << 8) | buff[9];
    gyy = (((int16_t)buff[10]) << 8) | buff[11];
    gzz = (((int16_t)buff[12]) << 8) | buff[13];

    *hx = (((int16_t)buff[15]) << 8) | buff[14];  
    *hy = (((int16_t)buff[17]) << 8) | buff[16];
    *hz = (((int16_t)buff[19]) << 8) | buff[18];

    *ax = tX[0]*axx + tX[1]*ayy + tX[2]*azz; // transform axes
    *ay = tY[0]*axx + tY[1]*ayy + tY[2]*azz;
    *az = tZ[0]*axx + tZ[1]*ayy + tZ[2]*azz;

    *gx = tX[0]*gxx + tX[1]*gyy + tX[2]*gzz;
    *gy = tY[0]*gxx + tY[1]*gyy + tY[2]*gzz;
    *gz = tZ[0]*gxx + tZ[1]*gyy + tZ[2]*gzz;
}

/* get accelerometer, gyro, and magnetometer data given pointers to store values */
void MPU9250::getMotion9(float* ax, float* ay, float* az, float* gx, float* gy, float* gz, float* hx, float* hy, float* hz){
    int16_t accel[3];
    int16_t gyro[3];
    int16_t mag[3];

    getMotion9Counts(&accel[0], &accel[1], &accel[2], &gyro[0], &gyro[1], &gyro[2], &mag[0], &mag[1], &mag[2]);

    *ax = ((float) accel[0]) * _accelScale; // typecast and scale to values
    *ay = ((float) accel[1]) * _accelScale;
    *az = ((float) accel[2]) * _accelScale;

    *gx = ((float) gyro[0]) * _gyroScale;
    *gy = ((float) gyro[1]) * _gyroScale;
    *gz = ((float) gyro[2]) * _gyroScale;

    *hx = ((float) mag[0]) * _magScaleX;
    *hy = ((float) mag[1]) * _magScaleY;
    *hz = ((float) mag[2]) * _magScaleZ;
}

/* get accelerometer, magnetometer, and temperature data given pointers to store values, return data as counts */
void MPU9250::getMotion10Counts(int16_t* ax, int16_t* ay, int16_t* az, int16_t* gx, int16_t* gy, int16_t* gz, int16_t* hx, int16_t* hy, int16_t* hz, int16_t* t){
    uint8_t buff[21];
    int16_t axx, ayy, azz, gxx, gyy, gzz;

    readRegisters(ACCEL_OUT, sizeof(buff), &buff[0]); // grab the data from the MPU9250

    axx = (((int16_t)buff[0]) << 8) | buff[1];  // combine into 16 bit values
    ayy = (((int16_t)buff[2]) << 8) | buff[3];
    azz = (((int16_t)buff[4]) << 8) | buff[5];

    *t = (((int16_t)buff[6]) << 8) | buff[7];

    gxx = (((int16_t)buff[8]) << 8) | buff[9];
    gyy = (((int16_t)buff[10]) << 8) | buff[11];
    gzz = (((int16_t)buff[12]) << 8) | buff[13];

    *hx = (((int16_t)buff[15]) << 8) | buff[14];
    *hy = (((int16_t)buff[17]) << 8) | buff[16];
    *hz = (((int16_t)buff[19]) << 8) | buff[18];

    *ax = tX[0]*axx + tX[1]*ayy + tX[2]*azz; // transform axes
    *ay = tY[0]*axx + tY[1]*ayy + tY[2]*azz;
    *az = tZ[0]*axx + tZ[1]*ayy + tZ[2]*azz;

    *gx = tX[0]*gxx + tX[1]*gyy + tX[2]*gzz;
    *gy = tY[0]*gxx + tY[1]*gyy + tY[2]*gzz;
    *gz = tZ[0]*gxx + tZ[1]*gyy + tZ[2]*gzz;
}

void MPU9250::getMotion10(float* ax, float* ay, float* az, float* gx, float* gy, float* gz, float* hx, float* hy, float* hz, float* t){
    int16_t accel[3];
    int16_t gyro[3];
    int16_t mag[3];
    int16_t tempCount;

    getMotion10Counts(&accel[0], &accel[1], &accel[2], &gyro[0], &gyro[1], &gyro[2], &mag[0], &mag[1], &mag[2], &tempCount);

    *ax = ((float) accel[0]) * _accelScale; // typecast and scale to values
    *ay = ((float) accel[1]) * _accelScale;
    *az = ((float) accel[2]) * _accelScale;

    *gx = ((float) gyro[0]) * _gyroScale;
    *gy = ((float) gyro[1]) * _gyroScale;
    *gz = ((float) gyro[2]) * _gyroScale;

    *hx = ((float) mag[0]) * _magScaleX;
    *hy = ((float) mag[1]) * _magScaleY;
    *hz = ((float) mag[2]) * _magScaleZ;

    *t = (( ((float) tempCount) - _tempOffset )/_tempScale) + _tempOffset; 
}

/* writes a byte to MPU9250 register given a register address and data */
bool MPU9250::writeRegister(uint8_t subAddress, uint8_t data){

   uint8_t buff2[2];
   buff2[0] = subAddress;
   buff2[1] = data;
   return SPIDataRW(_spi_bus, &buff2[0], 2) == 2;
}

/* reads registers from MPU9250 given a starting register address, number of bytes, and a pointer to store data */
void MPU9250::readRegisters(uint8_t subAddress, uint8_t count, uint8_t* dest){
        uint8_t buff2[2];
        for (uint8_t i=0; i<count; ++i) {
            buff2[0] = (subAddress+i) | 0x80;
            buff2[1] = 0;
            SPIDataRW(_spi_bus, &buff2[0], 2);
            dest[i] = buff2[1];
        }
}

/* writes a register to the AK8963 given a register address and data */
bool MPU9250::writeAK8963Register(uint8_t subAddress, uint8_t data){
	uint8_t count = 1;
	uint8_t buff[1];

	writeRegister(I2C_SLV0_ADDR,AK8963_I2C_ADDR); // set slave 0 to the AK8963 and set for write
	writeRegister(I2C_SLV0_REG,subAddress); // set the register to the desired AK8963 sub address
	writeRegister(I2C_SLV0_DO,data); // store the data for write
	writeRegister(I2C_SLV0_CTRL,I2C_SLV0_EN | count); // enable I2C and send 1 byte

	// read the register and confirm
	readAK8963Registers(subAddress, sizeof(buff), &buff[0]);

	return buff[0] == data;
}

/* reads registers from the AK8963 */
void MPU9250::readAK8963Registers(uint8_t subAddress, uint8_t count, uint8_t* dest){

	writeRegister(I2C_SLV0_ADDR,AK8963_I2C_ADDR | I2C_READ_FLAG); // set slave 0 to the AK8963 and set for read
	writeRegister(I2C_SLV0_REG,subAddress); // set the register to the desired AK8963 sub address
	writeRegister(I2C_SLV0_CTRL,I2C_SLV0_EN | count); // enable I2C and request the bytes
	usleep(100); // takes some time for these registers to fill
	readRegisters(EXT_SENS_DATA_00,count,dest); // read the bytes off the MPU9250 EXT_SENS_DATA registers
}

/* gets the MPU9250 WHO_AM_I register value, expected to be 0x71 */
uint8_t MPU9250::whoAmI(){
    uint8_t buff[1];

    // read the WHO AM I register
    readRegisters(WHO_AM_I,sizeof(buff),&buff[0]);

    // return the register value
    return buff[0];
}

/* gets the AK8963 WHO_AM_I register value, expected to be 0x48 */
uint8_t MPU9250::whoAmIAK8963(){
    uint8_t buff[1];

    // read the WHO AM I register
    readAK8963Registers(AK8963_WHO_AM_I,sizeof(buff),&buff[0]);

    // return the register value
    return buff[0];
}

uint8_t MPU9250::mpuWhoAmI()
{
    return whoAmI();
}

uint8_t MPU9250::magWhoAmI()
{
    return whoAmIAK8963();
}


// SPI


int MPU9250::SPIGetFd(int channel)
{
  return spiFds[channel & 1];
}


/*
 *  Write and Read a block of data over the SPI bus.
 *  Note the data ia being read into the transmit buffer, so will
 *  overwrite it!
 *  This is also a full-duplex operation.
 */
int MPU9250::SPIDataRW(int channel, uint8_t* data, int len)
{
  struct spi_ioc_transfer spi;

  channel &= 1;

  // Mentioned in spidev.h but not used in the original kernel documentation
  //  test program )-:

  memset(&spi, 0, sizeof (spi)) ;

  spi.tx_buf = (unsigned long) data;
  spi.rx_buf = (unsigned long) data;
  spi.len = len;
  spi.delay_usecs = spiDelay;
  spi.speed_hz = spiSpeeds [channel];
  spi.bits_per_word = spiBPW;

  return ioctl(spiFds [channel], SPI_IOC_MESSAGE(1), &spi);
}


/*
 *  Open the SPI device, and set it up, with the mode, etc.
 */
int MPU9250::SPISetupMode(int channel, int speed, int mode)
{
  int fd;

  mode &= 3; // Mode is 0, 1, 2 or 3
  channel &= 1; // Channel is 0 or 1

  if((fd = open(channel == 0 ? spiDev0 : spiDev1, O_RDWR)) < 0)
  {
    printf("Couldn't setup SPI for MPU-9250.\n");
    return -1;
  }

  spiSpeeds[channel] = speed;
  spiFds[channel] = fd;

  // Set SPI parameters.

  if (ioctl(fd, SPI_IOC_WR_MODE, &mode) < 0)
  {
    printf("SPI Mode Change failure for MPU-9250.\n");
    return -1;
  }
  
  if (ioctl(fd, SPI_IOC_WR_BITS_PER_WORD, &spiBPW) < 0)
  {
    printf("SPI BPW Change failure for MPU-9250.\n");
    return -1;
  }

  if (ioctl(fd, SPI_IOC_WR_MAX_SPEED_HZ, &speed) < 0)
  {
    printf("SPI Speed Change failure for MPU-9250.\n");
    return -1;
  }

  return fd;
}


/*
 *  Open the SPI device, and set it up, etc. in the default MODE 0
 */
int MPU9250::SPISetup(int channel, int speed)
{
  return SPISetupMode(channel, speed, 0);
}