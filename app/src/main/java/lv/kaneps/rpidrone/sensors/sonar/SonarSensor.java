package lv.kaneps.rpidrone.sensors.sonar;

import lv.kaneps.rpidrone.api.sensors.sonar.ISonarSensor;

public class SonarSensor implements ISonarSensor
{
	@Override
	public float altitude()
	{
		return 0;
	}

	@Override
	public float minAltitude()
	{
		return 0.02f;
	}

	@Override
	public float maxAltitude()
	{
		return 7.0f;
	}

	@Override
	public void release()
	{

	}
}
