package lv.kaneps.rpidrone.rotors;

import lv.kaneps.rpidrone.api.rotors.IRotor;
import lv.kaneps.rpidrone.api.rotors.RotorID;

public class Rotor implements IRotor
{
	private final RotorID id;
	private final ESC esc;

	public Rotor(RotorID id, ESC esc)
	{
		this.id = id;
		this.esc = esc;
	}

	@Override
	public void setSpeed(float percentage)
	{
		esc.output(percentage);
	}

	@Override
	public float currentSpeed()
	{
		return esc.currentSpeed();
	}

	@Override
	public void stop()
	{
		esc.outputNeutralMicros();
	}
}
