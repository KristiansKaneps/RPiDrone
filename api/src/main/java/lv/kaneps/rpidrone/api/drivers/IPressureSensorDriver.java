package lv.kaneps.rpidrone.api.drivers;

public interface IPressureSensorDriver extends IDriver
{
	/**
	 * @return current measured pressure in kPa
	 */
	float measurePressure();

	/**
	 * @return current measured temperature in °C
	 */
	float measureTemperature();

	/**
	 * @return current measured temperature in K
	 */
	float measureTemperatureK();

	/**
	 * @return current measured altitude in m
	 */
	float measureAltitude();
}
