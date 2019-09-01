#include "npwm.h"

JNIEXPORT void JNICALL Java_lv_kaneps_rpidrone_drivers_PWMDriver_init(JNIEnv*, jobject)
{
	/* configure library */
	gpioCfgBufferSize(PI_DEFAULT_BUFFER_MILLIS);
	gpioCfgClock(PI_DEFAULT_CLK_MICROS, PI_DEFAULT_CLK_PERIPHERAL, 0);
	gpioCfgInterfaces(PI_DEFAULT_IF_FLAGS);
	gpioCfgDMAchannels(PI_DEFAULT_DMA_PRIMARY_CHANNEL, PI_DEFAULT_DMA_SECONDARY_CHANNEL);
	gpioCfgSocketPort(PI_DEFAULT_SOCKET_PORT);
	gpioCfgMemAlloc(PI_DEFAULT_MEM_ALLOC_MODE);
	gpioCfgSetInternals(PI_DEFAULT_CFG_INTERNALS);

	/* init library */
	if (gpioInitialise() < 0) printf("Couldn't initialize PWM driver.\n");
}

JNIEXPORT void JNICALL Java_lv_kaneps_rpidrone_drivers_PWMDriver_release(JNIEnv* env, jobject obj)
{
  gpioTerminate();
}

JNIEXPORT void JNICALL Java_lv_kaneps_rpidrone_drivers_PWMDriver_output(JNIEnv* env, jobject obj, jint gpio, jint duty)
{
  int result = gpioPWM(gpio, duty);

	if(result == 0) return;

	switch(result)
	{
		case PI_BAD_USER_GPIO: return;
		case PI_BAD_DUTYCYCLE: return;
	}
}