package lv.kaneps.rpidrone.api.sensors.sonar;

import lv.kaneps.rpidrone.api.drivers.ISonarSensorDriver;
import lv.kaneps.rpidrone.api.sensors.INativeSensor;

public interface ISonarSensor extends INativeSensor<ISonarSensorDriver>
{
	float altitude();

	/**
	 * @return min altitude that sonar sensor can return
	 */
	float minAltitude();

	/**
	 * @return max altitude that sonar sensor can return
	 */
	float maxAltitude();
}
