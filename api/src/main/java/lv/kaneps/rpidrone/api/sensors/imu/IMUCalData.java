package lv.kaneps.rpidrone.api.sensors.imu;

import lv.kaneps.rpidrone.math.Vector3i;

public class IMUCalData
{
	public static final IMUCalData UNCALIBRATED = new IMUCalData();

	private static Vector3i ZERO = new Vector3i();

	public final int[] a, g, m;

	public IMUCalData(int[] a, int[] g, int[] m)
	{
		this.a = a == null ? ZERO.toArray() : a;
		this.g = g == null ? ZERO.toArray() : g;
		this.m = m == null ? ZERO.toArray() : m;
	}

	public IMUCalData(int[] a, int[] g)
	{
		this.a = a == null ? ZERO.toArray() : a;
		this.g = g == null ? ZERO.toArray() : g;
		m = ZERO.toArray();
	}

	public IMUCalData(int[] m)
	{
		a = ZERO.toArray();
		g = ZERO.toArray();
		this.m = m == null ? ZERO.toArray() : m;
	}

	public IMUCalData(Vector3i a, Vector3i g, Vector3i m)
	{
		this.a = a == null ? ZERO.toArray() : a.toArray();
		this.g = g == null ? ZERO.toArray() : g.toArray();
		this.m = m == null ? ZERO.toArray() : m.toArray();
	}

	public IMUCalData()
	{
		this.a = ZERO.toArray();
		this.g = ZERO.toArray();
		this.m = ZERO.toArray();
	}

	public void setAccel(int... a)
	{
		this.a[0] = a[0];
		this.a[1] = a[1];
		this.a[2] = a[2];
	}

	public void setAccel(Vector3i a)
	{
		setAccel(a.toArray());
	}

	public void setGyro(int... g)
	{
		this.g[0] = g[0];
		this.g[1] = g[1];
		this.g[2] = g[2];
	}

	public void setGyro(Vector3i g)
	{
		setGyro(g.toArray());
	}

	public void setMag(int... m)
	{
		this.m[0] = m[0];
		this.m[1] = m[1];
		this.m[2] = m[2];
	}

	public void setMag(Vector3i m)
	{
		setMag(m.toArray());
	}

	public Vector3i accelVec()
	{
		return new Vector3i(a);
	}

	public Vector3i gyroVec()
	{
		return new Vector3i(g);
	}

	public Vector3i magVec()
	{
		return new Vector3i(m);
	}

	@Override
	public String toString()
	{
		return "a[" + accelVec().toString() + "], g[" + gyroVec().toString() + "], m[" + magVec().toString() + ']';
	}
}
