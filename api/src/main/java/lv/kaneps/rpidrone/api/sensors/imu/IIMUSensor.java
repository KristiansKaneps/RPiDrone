package lv.kaneps.rpidrone.api.sensors.imu;

import lv.kaneps.rpidrone.api.drivers.IIMUSensorDriver;
import lv.kaneps.rpidrone.api.sensors.ICalibratedSensor;
import lv.kaneps.rpidrone.api.sensors.INativeSensor;
import lv.kaneps.rpidrone.math.Quaternion;
import lv.kaneps.rpidrone.math.Vector3f;

public interface IIMUSensor extends INativeSensor<IIMUSensorDriver>, ICalibratedSensor<IMUCalData, IMUCalType>
{
	float pitch();
	float yaw();
	float roll();

	Quaternion quaternion();

	Vector3f gyroscope();
	Vector3f accelerometer();
	Vector3f magnetometer();
}
