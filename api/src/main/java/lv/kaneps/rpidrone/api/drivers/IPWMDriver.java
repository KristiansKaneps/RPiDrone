package lv.kaneps.rpidrone.api.drivers;

public interface IPWMDriver extends IDriver
{
	/**
	 * @param gpio - PWM pin
	 * @param duty - duty cycle
	 */
	void output(int gpio, int duty);
}
