package lv.kaneps.rpidrone;

import lv.kaneps.rpidrone.api.IDrone;
import lv.kaneps.rpidrone.api.rotors.RotorID;
import lv.kaneps.rpidrone.api.sensors.imu.IMUCalType;
import lv.kaneps.rpidrone.drivers.IMUSensorDriver;
import lv.kaneps.rpidrone.drivers.PWMDriver;
import lv.kaneps.rpidrone.drivers.PressureSensorDriver;
import lv.kaneps.rpidrone.drivers.SonarSensorDriver;
import lv.kaneps.rpidrone.pid.MiniPID;
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
	private boolean alreadyInitialized = false;

	private Rotor[] rotors = new Rotor[4];
	private Sensors sensors;

	private PWMDriver pwm;

	private MiniPID pitchPid, rollPid, yawPid;

	protected Drone() {}

	protected synchronized final void init()
	{
		instance = this;
		if(alreadyInitialized) return;
		alreadyInitialized = true;

		pitchPid = new MiniPID(3, 0.048, 2);
		rollPid = new MiniPID(3, 0.048, 2);
		yawPid = new MiniPID(3, 0.048, 2);

		Log.i("Initializing rotors.");

		pwm = PWMDriver.instance();
		pwm.init();

		rotors[0] = new Rotor(RotorID.FRONT_RIGHT, new ESC(pwm, 0).init(escLogMsg));
		rotors[1] = new Rotor(RotorID.FRONT_LEFT, new ESC(pwm, 1).init(escLogMsg));
		rotors[2] = new Rotor(RotorID.BACK_LEFT, new ESC(pwm, 2).init(escLogMsg));
		rotors[3] = new Rotor(RotorID.BACK_RIGHT, new ESC(pwm, 3).init(escLogMsg));

		Log.i("Initializing sensors.");

		final IMUSensorDriver imuDriver = IMUSensorDriver.instance();
		final PressureSensorDriver pressureDriver = PressureSensorDriver.instance();
		final SonarSensorDriver sonarDriver = SonarSensorDriver.instance();

		imuDriver.init();
		pressureDriver.init();
		sonarDriver.init();

		final PressureSensor pressureSensor = new PressureSensor(pressureDriver);
		final SonarSensor sonarSensor = new SonarSensor(sonarDriver);

		sensors = new Sensors(
				new AltitudeSensor(pressureSensor, sonarSensor),
				new IMUSensor(imuDriver),
				new TemperatureSensor(pressureSensor),
				pressureSensor,
				sonarSensor
		);

		sensors.imuSensor().calMeasure(IMUCalType.ALL, 1000, data -> Log.i("IMU initialized with offsets: " + data.toString())).futureApply();

		Log.i("Drone initialized.");
	}

	protected synchronized final void start()
	{
		if(running) return;
		running = true;

		loopThread = new Thread(new DroneLoop(this), "Drone Loop");
		loopThread.start();
	}

	protected synchronized final void stop()
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
		pwm.release();
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

	@Override
	public void setPitchPID(MiniPID pid)
	{
		pitchPid = pid;
	}

	@Override
	public void setRollPID(MiniPID pid)
	{
		rollPid = pid;
	}

	@Override
	public void setYawPID(MiniPID pid)
	{
		yawPid = pid;
	}

	@Override
	public MiniPID getPitchPID()
	{
		return pitchPid;
	}

	@Override
	public MiniPID getRollPID()
	{
		return rollPid;
	}

	@Override
	public MiniPID getYawPID()
	{
		return yawPid;
	}
}
