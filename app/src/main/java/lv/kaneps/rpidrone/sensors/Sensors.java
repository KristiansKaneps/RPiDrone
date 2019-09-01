package lv.kaneps.rpidrone.sensors;

import lv.kaneps.rpidrone.api.sensors.IAltitudeSensor;
import lv.kaneps.rpidrone.api.sensors.imu.IIMUSensor;
import lv.kaneps.rpidrone.api.sensors.ISensors;
import lv.kaneps.rpidrone.api.sensors.ITemperatureSensor;
import lv.kaneps.rpidrone.api.sensors.pressure.IPressureSensor;
import lv.kaneps.rpidrone.api.sensors.sonar.ISonarSensor;
import lv.kaneps.rpidrone.sensors.imu.IMUSensor;
import lv.kaneps.rpidrone.sensors.pressure.PressureSensor;
import lv.kaneps.rpidrone.sensors.sonar.SonarSensor;

public class Sensors implements ISensors
{
	// made up sensors
	private final AltitudeSensor alt;
	private final TemperatureSensor temp;

	// real sensors
	private final IMUSensor imu;
	private final PressureSensor pressure;
	private final SonarSensor sonar;

	public Sensors(AltitudeSensor alt, IMUSensor imu, TemperatureSensor temp, PressureSensor pressure, SonarSensor sonar)
	{
		this.alt = alt;
		this.temp = temp;

		this.imu = imu;
		this.pressure = pressure;
		this.sonar = sonar;
	}

	@Override
	public void shutdownSensors()
	{
		imu.release();
		pressure.release();
		sonar.release();
	}

	@Override
	public IAltitudeSensor altitudeSensor()
	{
		return alt;
	}

	@Override
	public ITemperatureSensor temperatureSensor()
	{
		return temp;
	}

	@Override
	public IIMUSensor imuSensor()
	{
		return imu;
	}

	@Override
	public IPressureSensor pressureSensor()
	{
		return pressure;
	}

	@Override
	public ISonarSensor sonarSensor()
	{
		return sonar;
	}


}
