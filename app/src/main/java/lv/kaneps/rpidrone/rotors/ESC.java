package lv.kaneps.rpidrone.rotors;

import org.jetbrains.annotations.Nullable;

public class ESC
{
	public static final int NEUTRAL_MICROS = 1000;
	public static final int MAX_MICROS = 2000;
	public static final int ESC_INIT_MILLIS = 5400;

	private int currentMicros = NEUTRAL_MICROS;

	private boolean initializing = false;
	private boolean initialized = false;
	private Thread initThread;

	private final int gpio;

	public ESC(int gpio)
	{
		this.gpio = gpio;
	}

	/**
	 * @return the same instance of this ESC object
	 */
	public synchronized ESC init(@Nullable InitDelegate listener)
	{
		if(initialized || initializing) return this;
		initializing = true;

		initThread = new Thread(() -> {
			byte errorCode = 0;
			try
			{
				output(NEUTRAL_MICROS);
				Thread.sleep(ESC_INIT_MILLIS);
			}
			catch(InterruptedException e)
			{
				errorCode = 1;
				e.printStackTrace();
			}
			finally
			{
				initializing = false;
				initialized = true;

				if(listener != null)
					listener.onInitialized(ESC.this, errorCode);
			}
		}, "ESC @ " + gpio + " Init");
		initThread.start();
		return this;
	}

	/**
	 * @param micros - must be between NEUTRAL_MICROS and MAX_MICROS
	 */
	public void output(int micros)
	{
		this.currentMicros = micros;
		//pwm.output(gpio, micros); TODO
	}

	/**
	 * @param percentage - must be between 0.0f and 1.0f
	 */
	public void output(float percentage)
	{
		if(percentage > 1) percentage = 1;
		else if(percentage < 0) percentage = 0;
		int micros = (int) ((MAX_MICROS - NEUTRAL_MICROS) * percentage) + NEUTRAL_MICROS;
		output(Math.max(micros, MAX_MICROS));
	}

	public void outputNeutralMicros()
	{
		output(NEUTRAL_MICROS);
	}

	public void resetInit()
	{
		initialized = false;
	}

	public int getGPIO()
	{
		return gpio;
	}

	/**
	 * @return current ESC's PWM duty cycle
	 */
	public int currentMicros()
	{
		return this.currentMicros;
	}

	/**
	 * @return current ESC's PWM duty cycle in speed percentage, where 0% = NEUTRAL_MICROS and 100% = MAX_MICROS
	 */
	public float currentSpeed()
	{
		return ((float) (currentMicros - NEUTRAL_MICROS)) / (MAX_MICROS - NEUTRAL_MICROS);
	}

	@FunctionalInterface
	public interface InitDelegate
	{
		void onInitialized(ESC esc, byte errorCode);
	}

	@Override
	public String toString()
	{
		return "ESC @ GPIO Pin " + gpio + " (" + super.toString() + ')';
	}
}
