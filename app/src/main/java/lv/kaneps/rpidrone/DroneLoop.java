package lv.kaneps.rpidrone;

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

		while(drone.isRunning())
		{

		}
	}
}
