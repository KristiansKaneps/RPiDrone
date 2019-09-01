package lv.kaneps.rpidrone.api.sensors.pressure;

import lv.kaneps.rpidrone.api.drivers.IPressureSensorDriver;
import lv.kaneps.rpidrone.api.sensors.INativeSensor;

public interface IPressureSensor extends INativeSensor<IPressureSensorDriver>
{
	float pressure();
	float altitude();
	float temperature();
}
