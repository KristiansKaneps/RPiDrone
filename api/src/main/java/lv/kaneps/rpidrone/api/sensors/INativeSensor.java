package lv.kaneps.rpidrone.api.sensors;

import lv.kaneps.rpidrone.api.drivers.IDriver;

public interface INativeSensor<T extends IDriver>
{
	void release();

	T driver();
}
