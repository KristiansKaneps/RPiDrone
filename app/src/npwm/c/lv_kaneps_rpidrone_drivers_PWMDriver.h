#include <jni.h>

#ifndef _PWMDriver
#define _PWMDriver
#ifdef __cplusplus
extern "C" {
#endif

JNIEXPORT void JNICALL Java_lv_kaneps_rpidrone_drivers_PWMDriver_init(JNIEnv*, jobject);
JNIEXPORT void JNICALL Java_lv_kaneps_rpidrone_drivers_PWMDriver_release(JNIEnv*, jobject);

JNIEXPORT void JNICALL Java_lv_kaneps_rpidrone_drivers_PWMDriver_output(JNIEnv*, jobject, jint gpio, jint duty);

#ifdef __cplusplus
}
#endif
#endif