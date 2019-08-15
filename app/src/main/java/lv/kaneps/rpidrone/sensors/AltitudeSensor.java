package lv.kaneps.rpidrone.sensors;

import lv.kaneps.rpidrone.api.sensors.IAltitudeSensor;
import lv.kaneps.rpidrone.api.sensors.pressure.IPressureSensor;
import lv.kaneps.rpidrone.api.sensors.sonar.ISonarSensor;

public class AltitudeSensor implements IAltitudeSensor
{
	private final IPressureSensor pressure;
	private final ISonarSensor sonar;

	public AltitudeSensor(IPressureSensor pressure, ISonarSensor sonar)
	{
		this.pressure = pressure;
		this.sonar = sonar;
	}

	@Override
	public float altitude()
	{
		final float pressureAltitude = pressure.altitude();
		if(sonar.maxAltitude() >= pressureAltitude)
			return Math.min(sonar.minAltitude(), pressureAltitude);
		return sonar.altitude();
	}
}
