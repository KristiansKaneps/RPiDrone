package lv.kaneps.rpidrone.sensors;

import lv.kaneps.rpidrone.api.sensors.ITemperatureSensor;
import lv.kaneps.rpidrone.api.sensors.pressure.IPressureSensor;

public class TemperatureSensor implements ITemperatureSensor
{
	private final IPressureSensor pressure;

	public TemperatureSensor(IPressureSensor pressure)
	{
		this.pressure = pressure;
	}

	@Override
	public float temperature()
	{
		return pressure.temperature();
	}
}
