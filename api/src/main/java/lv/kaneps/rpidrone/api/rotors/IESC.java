package lv.kaneps.rpidrone.api.rotors;

import lv.kaneps.rpidrone.api.drivers.IPWMDriver;

public interface IESC
{
	/**
	 * Initialize this ESC with a guaranteed future notification which tells you when it is done
	 * @return the same instance of this ESC object
	 */
	IESC init(InitDelegate future);

	/**
	 * Initialize this ESC without any future notification
	 */
	IESC init();

	/**
	 * Call this only if you must re-initialize this ESC
	 */
	void resetInit();

	/**
	 * @param micros - must be between NEUTRAL_MICROS and MAX_MICROS
	 */
	void output(int micros);

	/**
	 * @param percentage - must be between 0.0f and 1.0f
	 */
	void output(float percentage);

	/**
	 * Stop this ESC
	 */
	void outputNeutralMicros();

	IPWMDriver getPWMDriver();

	/**
	 * @return GPIO pin this ESC is connected to
	 */
	int getGPIO();

	/**
	 * @return current ESC's PWM duty cycle
	 */
	int currentMicros();

	/**
	 * @return current ESC's PWM duty cycle in speed percentage, where 0% = NEUTRAL_MICROS and 100% = MAX_MICROS
	 */
	float currentSpeed();

	@FunctionalInterface
	interface InitDelegate
	{
		/**
		 * @param esc - potentially initialized ESC
		 * @param errorCode - it's set to 1 if there was an error during this ESC's initialization, 0 otherwise
		 */
		void onInitialized(IESC esc, byte errorCode);
	}
}
