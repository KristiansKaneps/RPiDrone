package lv.kaneps.rpidrone.sensors.imu;

import lv.kaneps.rpidrone.api.sensors.imu.IIMUSensor;
import lv.kaneps.rpidrone.math.Quaternion;
import lv.kaneps.rpidrone.math.Vector3f;

public class IMUSensor implements IIMUSensor
{
	public IMUSensor()
	{

	}

	@Override
	public float pitch()
	{
		return 0;
	}

	@Override
	public float yaw()
	{
		return 0;
	}

	@Override
	public float roll()
	{
		return 0;
	}

	@Override
	public Quaternion quaternion()
	{
		return null;
	}

	@Override
	public Vector3f gyroscope()
	{
		return null;
	}

	@Override
	public Vector3f accelerometer()
	{
		return null;
	}

	@Override
	public Vector3f magnetometer()
	{
		return null;
	}

	@Override
	public void release()
	{

	}
}
