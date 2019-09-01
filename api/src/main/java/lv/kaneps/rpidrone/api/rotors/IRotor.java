package lv.kaneps.rpidrone.api.rotors;

public interface IRotor
{
	/**
	 * @param percentage - speed in percents where 0% = stopped and 100% = full speed (must be between 0.0f and 1.0f)
	 */
	void setSpeed(float percentage);

	/**
	 * @return this rotor's ID
	 */
	RotorID id();

	/**
	 * @return this rotor's ESC
	 */
	IESC getESC();

	/**
	 * @return current speed in percentage
	 */
	default float currentSpeed() { return getESC().currentSpeed(); }

	/**
	 * Stops rotor
	 */
	default void stop() { getESC().outputNeutralMicros(); }
}
