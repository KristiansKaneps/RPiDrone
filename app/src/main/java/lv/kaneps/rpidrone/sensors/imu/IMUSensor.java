package lv.kaneps.rpidrone.sensors.imu;

import lv.kaneps.rpidrone.api.drivers.IIMUSensorDriver;
import lv.kaneps.rpidrone.api.sensors.imu.IIMUSensor;
import lv.kaneps.rpidrone.api.sensors.imu.IMUCalData;
import lv.kaneps.rpidrone.api.sensors.imu.IMUCalType;
import lv.kaneps.rpidrone.drivers.IMUSensorDriver;
import lv.kaneps.rpidrone.math.Quaternion;
import lv.kaneps.rpidrone.math.Vector3f;
import lv.kaneps.rpidrone.utils.ArrayDivUtils;
import org.jetbrains.annotations.Nullable;

import java.util.concurrent.atomic.AtomicBoolean;

public class IMUSensor implements IIMUSensor
{
	private IMUCalData measuredCalData = IMUCalData.UNCALIBRATED;

	private final AtomicBoolean measurementRunning = new AtomicBoolean(false);
	private final AtomicBoolean applyCalDataAfterMeasuring = new AtomicBoolean(false);

	private final IFutureApplyMeasuredCal futureApply = () -> applyCalDataAfterMeasuring.set(true);

	private final IMUSensorDriver driver;

	public IMUSensor(IMUSensorDriver driver)
	{
		this.driver = driver;
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
		driver.update();
		return new Vector3f(driver.getGyroscopeValues());
	}

	@Override
	public Vector3f accelerometer()
	{
		driver.update();
		return new Vector3f(driver.getAccelerometerValues());
	}

	@Override
	public Vector3f magnetometer()
	{
		driver.update();
		return new Vector3f(driver.getMagnetometerValues());
	}

	@Override
	public void release()
	{
		driver.release();
	}

	@Override
	public IIMUSensorDriver driver()
	{
		return driver;
	}

	@Override
	public void applyCustomCal(@Nullable IMUCalData calData)
	{
		driver.setCalData(calData == null ? new IMUCalData() : calData);
	}

	@Override
	public IMUCalData getAppliedCal()
	{
		return driver.getCalData();
	}

	@Override
	public IMUCalData getMeasuredCal()
	{
		return measuredCalData;
	}

	@Override
	public void setMeasuredCalData(@Nullable IMUCalData calData)
	{
		this.measuredCalData = calData == null ? IMUCalData.UNCALIBRATED : calData;
	}

	@Override
	public boolean isCalibrating()
	{
		return measurementRunning.get();
	}

	@Override
	public boolean isCalibrated()
	{
		return measuredCalData != IMUCalData.UNCALIBRATED;
	}

	@Override
	public synchronized IFutureApplyMeasuredCal calMeasure(IMUCalType type, int num, IFutureData<IMUCalData> calData)
	{
		if(measurementRunning.get()) return futureApply;
		measurementRunning.set(true);

		new Thread(() -> {
			long[] al = {0, 0, 0}, gl = {0, 0, 0}, ml = {0, 0, 0};
			for (int i = 0; i < num; i++)
			{
				if (type == IMUCalType.ACC || type == IMUCalType.ACC_GYR || type == IMUCalType.ALL)
				{
					int[] ac = driver.getAccelerometerValuesAsCounts();
					al[0] += ac[0];
					al[1] += ac[1];
					al[2] += ac[2];
				}

				if (type == IMUCalType.GYR || type == IMUCalType.ACC_GYR || type == IMUCalType.ALL)
				{
					int[] gc = driver.getGyroscopeValuesAsCounts();
					gl[0] += gc[0];
					gl[1] += gc[1];
					gl[2] += gc[2];
				}

				if (type == IMUCalType.MAG || type == IMUCalType.ALL)
				{
					int[] mc = driver.getMagnetometerValuesAsCounts();
					ml[0] += mc[0];
					ml[1] += mc[1];
					ml[2] += mc[2];
				}

				try { Thread.sleep(driver.getPollInterval()); } catch (InterruptedException e) { e.printStackTrace(); }
			}

			final IMUCalData data = new IMUCalData(ArrayDivUtils.div(al, num),
												   ArrayDivUtils.div(gl, num),
												   ArrayDivUtils.div(ml, num));
			IMUSensor.this.measuredCalData = data;
			if (calData != null) calData.dataReady(data);

			measurementRunning.set(false);

			if(applyCalDataAfterMeasuring.get())
				IMUSensor.this.applyMeasuredCal();
			applyCalDataAfterMeasuring.set(false);
		}, "IMU Calibration").start();

		return futureApply;
	}
}
