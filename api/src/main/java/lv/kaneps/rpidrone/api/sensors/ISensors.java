package lv.kaneps.rpidrone.api.sensors;

import lv.kaneps.rpidrone.api.sensors.imu.IIMUSensor;
import lv.kaneps.rpidrone.api.sensors.pressure.IPressureSensor;
import lv.kaneps.rpidrone.api.sensors.sonar.ISonarSensor;

public interface ISensors
{
	// Made up sensors (software sensors)
	IAltitudeSensor altitudeSensor();
	ITemperatureSensor temperatureSensor();

	// Real sensors (hardware sensors)
	IIMUSensor imuSensor();
	IPressureSensor pressureSensor();
	ISonarSensor sonarSensor();

	/**
	 * Automatically releases all native sensors (those that implement INativeSensor interface)
	 */
	void shutdownSensors();
}
