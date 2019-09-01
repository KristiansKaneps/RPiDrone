#include <jni.h>

#ifndef _SonarSensorDriver
#define _SonarSensorDriver
#ifdef __cplusplus
extern "C" {
#endif

JNIEXPORT jlong JNICALL Java_lv_kaneps_rpidrone_drivers_SonarSensorDriver_init(JNIEnv*, jobject, jint adapter);
JNIEXPORT void JNICALL Java_lv_kaneps_rpidrone_drivers_SonarSensorDriver_release(JNIEnv*, jobject, jlong p);

JNIEXPORT jfloat JNICALL Java_lv_kaneps_rpidrone_drivers_SonarSensorDriver_getMinAltitude(JNIEnv*, jobject, jlong p);
JNIEXPORT jfloat JNICALL Java_lv_kaneps_rpidrone_drivers_SonarSensorDriver_getMaxAltitude(JNIEnv*, jobject, jlong p);

JNIEXPORT jfloat JNICALL Java_lv_kaneps_rpidrone_drivers_SonarSensorDriver_getAltitude(JNIEnv*, jobject, jlong p);

#ifdef __cplusplus
}
#endif
#endif