#include <jni.h>

#ifndef _IMUSensorDriver
#define _IMUSensorDriver
#ifdef __cplusplus
extern "C" {
#endif

JNIEXPORT jlong JNICALL Java_lv_kaneps_rpidrone_drivers_IMUSensorDriver_init(JNIEnv*, jobject, jint oxIdx, jint oyIdx, jint ozIdx, jint accelRange, jint gyroRange, jint spiBus, jint spiSpeed);
JNIEXPORT void JNICALL Java_lv_kaneps_rpidrone_drivers_IMUSensorDriver_release(JNIEnv*, jobject, jlong p);

JNIEXPORT jint JNICALL Java_lv_kaneps_rpidrone_drivers_IMUSensorDriver_getPollInterval(JNIEnv*, jobject, jlong p);

JNIEXPORT void JNICALL Java_lv_kaneps_rpidrone_drivers_IMUSensorDriver_update(JNIEnv*, jobject, jlong p);
JNIEXPORT void JNICALL Java_lv_kaneps_rpidrone_drivers_IMUSensorDriver_updateCounts(JNIEnv*, jobject, jlong p);

JNIEXPORT jint JNICALL Java_lv_kaneps_rpidrone_drivers_IMUSensorDriver_whoAmIMPU9250(JNIEnv*, jobject, jlong p);
JNIEXPORT jint JNICALL Java_lv_kaneps_rpidrone_drivers_IMUSensorDriver_whoAmIAK8963(JNIEnv*, jobject, jlong p);

JNIEXPORT jfloat JNICALL Java_lv_kaneps_rpidrone_drivers_IMUSensorDriver_getTemperature(JNIEnv*, jobject, jlong p);
JNIEXPORT jfloat JNICALL Java_lv_kaneps_rpidrone_drivers_IMUSensorDriver_getTemperatureK(JNIEnv*, jobject, jlong p);

JNIEXPORT jfloatArray JNICALL Java_lv_kaneps_rpidrone_drivers_IMUSensorDriver_getAccelerometerValues(JNIEnv*, jobject, jlong p);
JNIEXPORT jfloatArray JNICALL Java_lv_kaneps_rpidrone_drivers_IMUSensorDriver_getGyroscopeValues(JNIEnv*, jobject, jlong p);
JNIEXPORT jfloatArray JNICALL Java_lv_kaneps_rpidrone_drivers_IMUSensorDriver_getMagnetometerValues(JNIEnv*, jobject, jlong p);

JNIEXPORT jintArray JNICALL Java_lv_kaneps_rpidrone_drivers_IMUSensorDriver_getAccelerometerValuesAsCounts(JNIEnv*, jobject, jlong p);
JNIEXPORT jintArray JNICALL Java_lv_kaneps_rpidrone_drivers_IMUSensorDriver_getGyroscopeValuesAsCounts(JNIEnv*, jobject, jlong p);
JNIEXPORT jintArray JNICALL Java_lv_kaneps_rpidrone_drivers_IMUSensorDriver_getMagnetometerValuesAsCounts(JNIEnv*, jobject, jlong p);

#ifdef __cplusplus
}
#endif
#endif