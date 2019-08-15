package lv.kaneps.rpidrone;

import lv.kaneps.rpidrone.api.IDrone;
import lv.kaneps.rpidrone.api.rotors.RotorID;
import lv.kaneps.rpidrone.rotors.ESC;
import lv.kaneps.rpidrone.rotors.Rotor;
import lv.kaneps.rpidrone.sensors.AltitudeSensor;
import lv.kaneps.rpidrone.sensors.imu.IMUSensor;
import lv.kaneps.rpidrone.sensors.Sensors;
import lv.kaneps.rpidrone.sensors.TemperatureSensor;
import lv.kaneps.rpidrone.sensors.pressure.PressureSensor;
import lv.kaneps.rpidrone.sensors.sonar.SonarSensor;

public class Drone implements IDrone
{
	private static Drone instance;

	public static IDrone instance()
	{
		if(instance == null) throw new RuntimeException("Drone is not initialized");
		return instance;
	}

	private static final ESC.InitDelegate escLogMsg = (esc, err) -> Log.i(esc.toString() + " initialized with error code " + err + '.');

	private Thread loopThread;
	private boolean running = false;

	private Rotor[] rotors = new Rotor[4];
	private Sensors sensors;

	protected Drone() {}

	protected final void init()
	{
		instance = this;

		Log.i("Initializing rotors.");

		rotors[0] = new Rotor(RotorID.FRONT_RIGHT, new ESC(0).init(escLogMsg));
		rotors[1] = new Rotor(RotorID.FRONT_LEFT, new ESC(1).init(escLogMsg));
		rotors[2] = new Rotor(RotorID.BACK_LEFT, new ESC(2).init(escLogMsg));
		rotors[3] = new Rotor(RotorID.BACK_RIGHT, new ESC(3).init(escLogMsg));

		Log.i("Initializing sensors.");

		final PressureSensor pressureSensor = new PressureSensor();
		final SonarSensor sonarSensor = new SonarSensor();

		sensors = new Sensors(
				new AltitudeSensor(pressureSensor, sonarSensor),
				new IMUSensor(),
				new TemperatureSensor(pressureSensor),
				pressureSensor,
				sonarSensor
		);

		Log.i("Drone initialized.");
	}

	protected final void start()
	{
		if(running) return;
		running = true;

		loopThread = new Thread(new DroneLoop(this), "Drone Loop");
		loopThread.start();
	}

	protected final void stop()
	{
		if(!running) return;
		running = false;

		try
		{
			loopThread.wait();
		}
		catch(InterruptedException e)
		{
			e.printStackTrace();
		}

		sensors.shutdownSensors();
	}

	@Override
	public boolean isRunning()
	{
		return running;
	}

	@Override
	public Sensors sensors()
	{
		return sensors;
	}

	@Override
	public Rotor[] rotors()
	{
		return rotors;
	}

	@Override
	public Rotor rotor(RotorID rotorId)
	{
		return rotors[rotorId.ordinal()];
	}
}
