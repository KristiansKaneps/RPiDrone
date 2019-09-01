package lv.kaneps.rpidrone.drivers;

import lv.kaneps.rpidrone.api.drivers.IPressureSensorDriver;

public class PressureSensorDriver implements IPressureSensorDriver
{
	static {
		System.loadLibrary("npressure");
	}

	private static PressureSensorDriver instance;

	public static PressureSensorDriver instance()
	{
		if(instance == null)
			instance = new PressureSensorDriver();
		return instance;
	}

	private long p;

	private final Mode mode;

	private PressureSensorDriver()
	{
		mode = Mode.BMP085_MODE_ULTRAHIGHRES;
	}

	public Mode getMode()
	{
		return mode;
	}

	public synchronized void init()
	{
		if(p != 0) return;
		p = init(mode.ordinal());
	}

	public synchronized void release()
	{
		if(p == 0) return;
		release(p);
		p = 0;
	}

	/**
	 * @return current pressure in kPa
	 */
	@Override
	public float measurePressure()
	{
		return getPressure(p);
	}

	/**
	 * @return current temperature in °C
	 */
	@Override
	public float measureTemperature()
	{
		return getTemperature(p);
	}

	/**
	 * @return current temperature in K
	 */
	@Override
	public float measureTemperatureK()
	{
		return getTemperatureK(p);
	}

	/**
	 * @return current altitude in m
	 */
	@Override
	public float measureAltitude()
	{
		return getAltitude(p);
	}

	private native long init(int mode);
	private native void release(long p);

	private native float getPressure(long p);
	private native float getTemperature(long p);
	private native float getTemperatureK(long p);
	private native float getAltitude(long p);

	public enum Mode
	{
		BMP085_MODE_ULTRALOWPOWER,
		BMP085_MODE_STANDARD,
		BMP085_MODE_HIGHRES,
		BMP085_MODE_ULTRAHIGHRES
	}
}
