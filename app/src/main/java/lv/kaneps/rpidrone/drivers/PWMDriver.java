package lv.kaneps.rpidrone.drivers;

import lv.kaneps.rpidrone.api.drivers.IPWMDriver;

/**
 * As this driver controls more than one GPIO pin, it must be released only one time because release() releases all controllable GPIO pins.
 */
public class PWMDriver implements IPWMDriver
{
	static {
		System.loadLibrary("npwm");
	}

	private static PWMDriver instance;

	public static PWMDriver instance()
	{
		if(instance == null)
			instance = new PWMDriver();
		return instance;
	}

	private PWMDriver() {}

	public native void init();
	public native void release();

	/**
	 * @param gpio - PWM pin
	 * @param duty - duty cycle
	 */
	@Override
	public native void output(int gpio, int duty);
}
