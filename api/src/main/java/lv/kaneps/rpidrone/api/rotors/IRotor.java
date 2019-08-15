package lv.kaneps.rpidrone.api.rotors;

public interface IRotor
{
	void setSpeed(float percentage);

	/**
	 * @return current speed in percentage
	 */
	float currentSpeed();

	void stop();
}
