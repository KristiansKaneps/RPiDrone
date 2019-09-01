package lv.kaneps.rpidrone.api.drivers;

import lv.kaneps.rpidrone.api.sensors.imu.IMUCalData;

public interface IIMUSensorDriver extends IDriver
{
	float[] getAccelerometerValues();

	float[] getGyroscopeValues();

	float[] getMagnetometerValues();

	/**
	 * @return accelerometer measured values as counts (raw measured data)
	 */
	int[] getAccelerometerValuesAsCounts();

	/**
	 * @return gyroscope measured values as counts (raw measured data)
	 */
	int[] getGyroscopeValuesAsCounts();

	/**
	 * @return magnetometer measured values as counts (raw measured data)
	 */
	int[] getMagnetometerValuesAsCounts();

	/**
	 * Set IMU calibration data
	 */
	void setCalData(IMUCalData calData);

	/**
	 * @return applied IMU calibration data
	 */
	IMUCalData getCalData();
}
