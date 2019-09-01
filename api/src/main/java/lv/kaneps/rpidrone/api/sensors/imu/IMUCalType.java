package lv.kaneps.rpidrone.api.sensors.imu;

import lv.kaneps.rpidrone.api.sensors.ICalibratedSensor;

public enum IMUCalType implements ICalibratedSensor.ICalTypeEnum
{
	ACC, MAG, GYR, ACC_GYR, ALL
}