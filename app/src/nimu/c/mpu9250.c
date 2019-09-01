#include "mpu9250.h"
#include <stdio.h>

#define init_successful (imu.status==0)

struct IMUp
{
    MPU9250* mpu;
    int8_t status;
    uint8_t oX = 0;
    uint8_t oY = 1;
    uint8_t oZ = 2;

    float a[3] = {0, 0, 0};
    float g[3] = {0, 0, 0};
    float m[3] = {0, 0, 0};
    float t = 0;

    int32_t ac[3] = {0, 0, 0};
    int32_t gc[3] = {0, 0, 0};
    int32_t mc[3] = {0, 0, 0};
    int32_t tc = 0;
};

JNIEXPORT jlong JNICALL Java_lv_kaneps_rpidrone_drivers_IMUSensorDriver_init(JNIEnv* env, jobject obj, jint oxIdx, jint oyIdx, jint ozIdx, jint accelRange, jint gyroRange, jint spiBus, jint spiSpeed)
{
  IMUp* imu = new IMUp();

	imu->oX = (uint8_t) oxIdx;
	imu->oY = (uint8_t) oyIdx;
	imu->oZ = (uint8_t) ozIdx;

	imu->mpu = new MPU9250((uint8_t) spiBus, (uint32_t) spiSpeed);

	// start communication with IMU and
	// set the accelerometer and gyro ranges.
	// ACCELEROMETER 2G 4G 8G 16G
	// GYRO 250DPS 500DPS 1000DPS 2000DPS
	imu->status = imu->mpu->begin((mpu9250_accel_range) accelRange, (mpu9250_gyro_range) gyroRange);

	if(imu->status < 0)
	{
		printf("IMU initialization unsuccessful.\n");
		delete imu->mpu;
		delete imu;
		return 0L;
	}

	return (long) imu;
}

JNIEXPORT void JNICALL Java_lv_kaneps_rpidrone_drivers_IMUSensorDriver_release(JNIEnv* env, jobject obj, jlong p)
{
  if(p == 0) return;
  IMUp* imu = (IMUp*) p;
  delete imu->mpu;
  delete imu;
}

JNIEXPORT jint JNICALL Java_lv_kaneps_rpidrone_drivers_IMUSensorDriver_getPollInterval(JNIEnv* env, jobject obj, jlong p)
{
  return 1;
}

JNIEXPORT void JNICALL Java_lv_kaneps_rpidrone_drivers_IMUSensorDriver_update(JNIEnv* env, jobject obj, jlong p)
{
  if(p == 0) return;
  IMUp imu = *((IMUp*) p);
  if(!init_successful) return;

  imu.mpu->getMotion10(&imu.a[imu.oX], &imu.a[imu.oY], &imu.a[imu.oZ], &imu.g[imu.oX], &imu.g[imu.oY], &imu.g[imu.oZ], &imu.m[imu.oX], &imu.m[imu.oY], &imu.m[imu.oZ], &imu.t);
}

JNIEXPORT void JNICALL Java_lv_kaneps_rpidrone_drivers_IMUSensorDriver_updateCounts(JNIEnv* env, jobject obj, jlong p)
{
  if(p == 0) return;
  IMUp imu = *((IMUp*) p);
  if(!init_successful) return;

  imu.mpu->getMotion10Counts(&imu.ac[imu.oX], &imu.ac[imu.oY], &imu.ac[imu.oZ], &imu.gc[imu.oX], &imu.gc[imu.oY], &imu.gc[imu.oZ], &imu.mc[imu.oX], &imu.mc[imu.oY], &imu.mc[imu.oZ], &imu.tc);
}

JNIEXPORT jint JNICALL Java_lv_kaneps_rpidrone_drivers_IMUSensorDriver_whoAmIMPU9250(JNIEnv* env, jobject obj, jlong p)
{
  if(p == 0) return 0;
  IMUp imu = *((IMUp*) p);
  if(!init_successful) return 0;

  return (int) imu.mpu->mpuWhoAmI();
}

JNIEXPORT jint JNICALL Java_lv_kaneps_rpidrone_drivers_IMUSensorDriver_whoAmIAK8963(JNIEnv* env, jobject obj, jlong p)
{
  if(p == 0) return 0;
  IMUp imu = *((IMUp*) p);
  if(!init_successful) return 0;

  return (int) imu.mpu->magWhoAmI();
}

JNIEXPORT jfloat JNICALL Java_lv_kaneps_rpidrone_drivers_IMUSensorDriver_getTemperature(JNIEnv* env, jobject obj, jlong p)
{
  if(p == 0) return -273.15f;
	IMUp imu = *((IMUp*) p);
	if(!init_successful) return -273.15f;

	return imu.t;
}

JNIEXPORT jfloat JNICALL Java_lv_kaneps_rpidrone_drivers_IMUSensorDriver_getTemperatureK(JNIEnv* env, jobject obj, jlong p)
{
  if(p == 0) return 0.0f;
	IMUp imu = *((IMUp*) p);
	if(!init_successful) return 0.0f;

	return imu.t + 273.15f;
}

JNIEXPORT jfloatArray JNICALL Java_lv_kaneps_rpidrone_drivers_IMUSensorDriver_getAccelerometerValues(JNIEnv* env, jobject obj, jlong p)
{
  if(p == 0) return NULL;
	IMUp imu = *((IMUp*) p);
	if(!init_successful) return NULL;

	jfloatArray result;
  result = env->NewFloatArray(3);
  if (result == NULL) return NULL; // out of memory
  env->SetFloatArrayRegion(result, 0, 3, &imu.a[0]);

  return result;
}

JNIEXPORT jfloatArray JNICALL Java_lv_kaneps_rpidrone_drivers_IMUSensorDriver_getGyroscopeValues(JNIEnv* env, jobject obj, jlong p)
{
  if(p == 0) return NULL;
	IMUp imu = *((IMUp*) p);
	if(!init_successful) return NULL;

	jfloatArray result;
  result = env->NewFloatArray(3);
  if (result == NULL) return NULL; // out of memory
  env->SetFloatArrayRegion(result, 0, 3, &imu.g[0]);

  return result;
}

JNIEXPORT jfloatArray JNICALL Java_lv_kaneps_rpidrone_drivers_IMUSensorDriver_getMagnetometerValues(JNIEnv* env, jobject obj, jlong p)
{
  if(p == 0) return NULL;
	IMUp imu = *((IMUp*) p);
	if(!init_successful) return NULL;

	jfloatArray result;
  result = env->NewFloatArray(3);
  if (result == NULL) return NULL; // out of memory
  env->SetFloatArrayRegion(result, 0, 3, &imu.m[0]);

  return result;
}

JNIEXPORT jintArray JNICALL Java_lv_kaneps_rpidrone_drivers_IMUSensorDriver_getAccelerometerValuesAsCounts(JNIEnv* env, jobject obj, jlong p)
{
  if(p == 0) return NULL;
	IMUp imu = *((IMUp*) p);
	if(!init_successful) return NULL;

	jintArray result;
  result = env->NewIntArray(3);
  if (result == NULL) return NULL; // out of memory
  env->SetIntArrayRegion(result, 0, 3, &imu.ac[0]);

  return result;
}

JNIEXPORT jintArray JNICALL Java_lv_kaneps_rpidrone_drivers_IMUSensorDriver_getGyroscopeValuesAsCounts(JNIEnv* env, jobject obj, jlong p)
{
  if(p == 0) return NULL;
	IMUp imu = *((IMUp*) p);
	if(!init_successful) return NULL;

	jintArray result;
  result = env->NewIntArray(3);
  if (result == NULL) return NULL; // out of memory
  env->SetIntArrayRegion(result, 0, 3, &imu.gc[0]);

  return result;
}

JNIEXPORT jintArray JNICALL Java_lv_kaneps_rpidrone_drivers_IMUSensorDriver_getMagnetometerValuesAsCounts(JNIEnv* env, jobject obj, jlong p)
{
  if(p == 0) return NULL;
	IMUp imu = *((IMUp*) p);
	if(!init_successful) return NULL;

	jintArray result;
  result = env->NewIntArray(3);
  if (result == NULL) return NULL; // out of memory
  env->SetIntArrayRegion(result, 0, 3, &imu.mc[0]);

  return result;
}