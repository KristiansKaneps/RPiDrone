package lv.kaneps.rpidrone.api;

import lv.kaneps.rpidrone.api.rotors.IRotor;
import lv.kaneps.rpidrone.api.rotors.RotorID;
import lv.kaneps.rpidrone.api.sensors.ISensors;
import lv.kaneps.rpidrone.pid.MiniPID;

public interface IDrone
{
	boolean isRunning();

	ISensors sensors();

	IRotor[] rotors();
	IRotor rotor(RotorID rotorId);

	void setPitchPID(MiniPID pid);
	void setRollPID(MiniPID pid);
	void setYawPID(MiniPID pid);

	MiniPID getPitchPID();
	MiniPID getRollPID();
	MiniPID getYawPID();
}
