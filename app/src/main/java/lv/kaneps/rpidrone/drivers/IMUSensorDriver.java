package lv.kaneps.rpidrone.drivers;

import lv.kaneps.rpidrone.api.drivers.IIMUSensorDriver;
import lv.kaneps.rpidrone.api.sensors.imu.IMUCalData;

import static lv.kaneps.rpidrone.drivers.IMUSensorDriver.AccelRange.ACCEL_RANGE_8G;
import static lv.kaneps.rpidrone.drivers.IMUSensorDriver.GyroRange.GYRO_RANGE_500DPS;

public class IMUSensorDriver implements IIMUSensorDriver
{
	static {
		System.loadLibrary("nimu");
	}

	private static IMUSensorDriver instance;

	public static IMUSensorDriver instance()
	{
		if(instance == null)
			instance = new IMUSensorDriver();
		return instance;
	}

	private long p;

	private long lastUpdate = System.currentTimeMillis();

	private IMUCalData calData = new IMUCalData();

	private final GyroRange gyroRange;
	private final AccelRange accelRange;
	private final int spiBus;
	private final int spiSpeed;

	private IMUSensorDriver()
	{
		gyroRange = GYRO_RANGE_500DPS;
		accelRange = ACCEL_RANGE_8G;
		spiBus = 0;
		spiSpeed = 400000;
	}

	public synchronized void init()
	{
		if(p != 0) return;
		p = init(0, 1, 2, accelRange.ordinal(), gyroRange.ordinal(), spiBus, spiSpeed);
	}

	public synchronized void release()
	{
		if(p == 0) return;
		release(p);
		p = 0;
	}

	/**
	 * @return min data poll interval in ms
	 */
	public int getPollInterval()
	{
		return getPollInterval(p);
	}

	/**
	 * Read measurements and store them.
	 */
	public void update()
	{
		long now = System.currentTimeMillis();
		if(now - lastUpdate >= getPollInterval())
		{
			lastUpdate = now;
			update(p);
		}
	}

	/**
	 * Read measurements as counts and store them (raw measured data)
	 */
	public void updateCounts()
	{
		long now = System.currentTimeMillis();
		if(now - lastUpdate >= getPollInterval())
		{
			lastUpdate = now;
			updateCounts(p);
		}
	}

	/**
	 * @return the MPU9250 WHO_AM_I register value, expected to be 0x71 (decimal 113)
	 */
	public int whoAmIMPU9250()
	{
		return whoAmIMPU9250(p);
	}

	/**
	 * @return the AK8963 WHO_AM_I register value, expected to be 0x48 (decimal 72)
	 */
	public int whoAmIAK8963()
	{
		return whoAmIAK8963(p);
	}

	/**
	 * @return measured temperature in °C
	 */
	public float getTemperature()
	{
		return getTemperature(p);
	}

	/**
	 * @return measured temperature in K
	 */
	public float getTemperatureK()
	{
		return getTemperatureK(p);
	}

	@Override
	public float[] getAccelerometerValues()
	{
		return getAccelerometerValues(p);
	}

	@Override
	public float[] getGyroscopeValues()
	{
		return getGyroscopeValues(p);
	}

	@Override
	public float[] getMagnetometerValues()
	{
		return getMagnetometerValues(p);
	}

	/**
	 * @return accelerometer measured values as counts (raw measured data)
	 */
	@Override
	public int[] getAccelerometerValuesAsCounts()
	{
		return getAccelerometerValuesAsCounts(p);
	}

	/**
	 * @return gyroscope measured values as counts (raw measured data)
	 */
	@Override
	public int[] getGyroscopeValuesAsCounts()
	{
		return getGyroscopeValuesAsCounts(p);
	}

	/**
	 * @return magnetometer measured values as counts (raw measured data)
	 */
	@Override
	public int[] getMagnetometerValuesAsCounts()
	{
		return getMagnetometerValuesAsCounts(p);
	}

	@Override
	public void setCalData(IMUCalData calData)
	{
		this.calData = calData;
	}

	@Override
	public IMUCalData getCalData()
	{
		return calData;
	}

	private native long init(int oxIdx, int oyIdx, int ozIdx, int accelRange, int gyroRange, int spiBus, int spiSpeed);
	private native void release(long p);

	private native int getPollInterval(long p);

	private native void update(long p);
	private native void updateCounts(long p);

	private native int whoAmIMPU9250(long p);
	private native int whoAmIAK8963(long p);

	private native float getTemperature(long p);
	private native float getTemperatureK(long p);

	private native float[] getAccelerometerValues(long p);
	private native float[] getGyroscopeValues(long p);
	private native float[] getMagnetometerValues(long p);

	private native int[] getAccelerometerValuesAsCounts(long p);
	private native int[] getGyroscopeValuesAsCounts(long p);
	private native int[] getMagnetometerValuesAsCounts(long p);

	public enum GyroRange
	{
		GYRO_RANGE_250DPS,
		GYRO_RANGE_500DPS,
		GYRO_RANGE_1000DPS,
		GYRO_RANGE_2000DPS
	}

	public enum AccelRange
	{
		ACCEL_RANGE_2G,
		ACCEL_RANGE_4G,
		ACCEL_RANGE_8G,
		ACCEL_RANGE_16G
	}
}
