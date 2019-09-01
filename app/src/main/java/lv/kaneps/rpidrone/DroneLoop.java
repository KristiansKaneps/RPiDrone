package lv.kaneps.rpidrone;

import lv.kaneps.rpidrone.api.sensors.imu.IIMUSensor;
import lv.kaneps.rpidrone.pid.MiniPID;
import lv.kaneps.rpidrone.rotors.Rotor;
import lv.kaneps.rpidrone.sensors.Sensors;

public final class DroneLoop implements Runnable
{
	private final Drone drone;

	protected DroneLoop(Drone drone)
	{
		this.drone = drone;
	}

	@Override
	public void run()
	{
		final Sensors s = drone.sensors();
		final Rotor[] rs = drone.rotors();

		final IIMUSensor imu = s.imuSensor();

		final MiniPID pitchPid = drone.getPitchPID();
		final MiniPID rollPid = drone.getRollPID();

		pitchPid.setSetpoint(0);
		rollPid.setSetpoint(0);

		while(drone.isRunning())
		{
			float pitch = imu.pitch();
			float roll = imu.roll();

			double pitchOut = pitchPid.getOutput(pitch);
			double rollOut = rollPid.getOutput(roll);

			Log.i("Pitch PID: " + pitchOut);
			Log.i("Roll PID: " + rollOut);
			Log.i("===");
		}
	}
}
