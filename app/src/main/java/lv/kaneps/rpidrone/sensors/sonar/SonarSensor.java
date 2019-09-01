package lv.kaneps.rpidrone.sensors.sonar;

import lv.kaneps.rpidrone.api.drivers.ISonarSensorDriver;
import lv.kaneps.rpidrone.api.sensors.sonar.ISonarSensor;
import lv.kaneps.rpidrone.drivers.SonarSensorDriver;

public class SonarSensor implements ISonarSensor
{
	private final SonarSensorDriver driver;

	public SonarSensor(SonarSensorDriver driver)
	{
		this.driver = driver;
	}

	@Override
	public float altitude()
	{
		return driver.measureAltitude();
	}

	@Override
	public float minAltitude()
	{
		return driver.getMinAltitude();
	}

	@Override
	public float maxAltitude()
	{
		return driver.getMaxAltitude();
	}

	@Override
	public void release()
	{
		driver.release();
	}

	@Override
	public ISonarSensorDriver driver()
	{
		return driver;
	}
}
