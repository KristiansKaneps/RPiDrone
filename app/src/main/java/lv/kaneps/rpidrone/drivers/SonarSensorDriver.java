package lv.kaneps.rpidrone.drivers;

import lv.kaneps.rpidrone.api.drivers.ISonarSensorDriver;

public class SonarSensorDriver implements ISonarSensorDriver
{
	static {
		System.loadLibrary("nsonar");
	}

	private static SonarSensorDriver instance;

	public static SonarSensorDriver instance()
	{
		if(instance == null)
			instance = new SonarSensorDriver(1);
		return instance;
	}

	private long p = 0;

	private final int adapter;

	private SonarSensorDriver(int adapter)
	{
		this.adapter = adapter;
	}

	public synchronized void init()
	{
		if(p != 0) return;
		p = init(adapter);
	}

	public synchronized void release()
	{
		if(p == 0) return;
		release(p);
		p = 0;
	}

	/**
	 * @return min possible altitude for this sensor to detect
	 */
	@Override
	public float getMinAltitude()
	{
		return getMinAltitude(p);
	}

	/**
	 * @return max possible altitude for this sensor to detect
	 */
	@Override
	public float getMaxAltitude()
	{
		return getMaxAltitude(p);
	}

	/**
	 * @return current altitude in m
	 */
	@Override
	public float measureAltitude()
	{
		return getAltitude(p);
	}

	private native long init(int adapter);
	private native void release(long p);

	private native float getMinAltitude(long p);
	private native float getMaxAltitude(long p);

	private native float getAltitude(long p);
}
