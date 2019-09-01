package lv.kaneps.rpidrone.sensors.pressure;

import lv.kaneps.rpidrone.api.drivers.IPressureSensorDriver;
import lv.kaneps.rpidrone.api.sensors.pressure.IPressureSensor;
import lv.kaneps.rpidrone.drivers.PressureSensorDriver;

public class PressureSensor implements IPressureSensor
{
	private final PressureSensorDriver driver;

	public PressureSensor(PressureSensorDriver driver)
	{
		this.driver = driver;
	}

	@Override
	public float pressure()
	{
		return driver.measurePressure();
	}

	@Override
	public float altitude()
	{
		return driver.measureAltitude();
	}

	@Override
	public float temperature()
	{
		return driver.measureTemperature();
	}

	@Override
	public void release()
	{
		driver.release();
	}

	@Override
	public IPressureSensorDriver driver()
	{
		return driver;
	}
}
