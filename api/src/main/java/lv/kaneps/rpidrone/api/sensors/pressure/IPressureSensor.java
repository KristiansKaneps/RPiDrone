package lv.kaneps.rpidrone.api.sensors.pressure;

import lv.kaneps.rpidrone.api.sensors.INativeSensor;

public interface IPressureSensor extends INativeSensor
{
	float pressure();
	float altitude();
	float temperature();
}
