package lv.kaneps.rpidrone.api.sensors;

import lv.kaneps.rpidrone.api.sensors.imu.IIMUSensor;
import lv.kaneps.rpidrone.api.sensors.pressure.IPressureSensor;
import lv.kaneps.rpidrone.api.sensors.sonar.ISonarSensor;

public interface ISensors
{
	IAltitudeSensor altitudeSensor();
	IIMUSensor imuSensor();
	ITemperatureSensor temperatureSensor();

	IPressureSensor pressureSensor();
	ISonarSensor sonarSensor();

	void shutdownSensors();
}
