#include <jni.h>

#ifndef _PressureSensorDriver
#define _PressureSensorDriver
#ifdef __cplusplus
extern "C" {
#endif

JNIEXPORT jlong JNICALL Java_lv_kaneps_rpidrone_drivers_PressureSensorDriver_init(JNIEnv*, jobject, jint mode);
JNIEXPORT void JNICALL Java_lv_kaneps_rpidrone_drivers_PressureSensorDriver_release(JNIEnv*, jobject, jlong p);

JNIEXPORT jfloat JNICALL Java_lv_kaneps_rpidrone_drivers_PressureSensorDriver_getPressure(JNIEnv*, jobject, jlong p);
JNIEXPORT jfloat JNICALL Java_lv_kaneps_rpidrone_drivers_PressureSensorDriver_getTemperature(JNIEnv*, jobject, jlong p);
JNIEXPORT jfloat JNICALL Java_lv_kaneps_rpidrone_drivers_PressureSensorDriver_getTemperatureK(JNIEnv*, jobject, jlong p);
JNIEXPORT jfloat JNICALL Java_lv_kaneps_rpidrone_drivers_PressureSensorDriver_getAltitude(JNIEnv*, jobject, jlong p);


#ifdef __cplusplus
}
#endif
#endif