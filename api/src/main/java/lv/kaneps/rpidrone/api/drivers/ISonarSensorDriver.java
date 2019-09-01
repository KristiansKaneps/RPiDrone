package lv.kaneps.rpidrone.api.drivers;

public interface ISonarSensorDriver extends IDriver
{
	/**
	 * @return min possible altitude for this sensor to detect
	 */
	float getMinAltitude();

	/**
	 * @return max possible altitude for this sensor to detect
	 */
	float getMaxAltitude();

	/**
	 * @return current measured altitude in m
	 */
	float measureAltitude();
}
