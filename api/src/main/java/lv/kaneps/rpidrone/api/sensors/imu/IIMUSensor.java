package lv.kaneps.rpidrone.api.sensors.imu;

import lv.kaneps.rpidrone.api.INativeSensor;
import lv.kaneps.rpidrone.math.Quaternion;
import lv.kaneps.rpidrone.math.Vector3f;

public interface IIMUSensor extends INativeSensor
{
	float pitch();
	float yaw();
	float roll();

	Quaternion quaternion();

	Vector3f gyroscope();
	Vector3f accelerometer();
	Vector3f magnetometer();
}
