package lv.kaneps.rpidrone.api;

import lv.kaneps.rpidrone.api.rotors.IRotor;
import lv.kaneps.rpidrone.api.rotors.RotorID;
import lv.kaneps.rpidrone.api.sensors.ISensors;

public interface IDrone
{
	boolean isRunning();

	ISensors sensors();

	IRotor[] rotors();
	IRotor rotor(RotorID rotorId);
}
