package lv.kaneps.rpidrone.math;

public class Vector3d
{
	public double x, y, z;

	public Vector3d(double x, double y, double z)
	{
		this.x = x;
		this.y = y;
		this.z = z;
	}

	public Vector3d()
	{
		this(0, 0, 0);
	}

	public double length()
	{
		return Math.sqrt(x * x + y * y + z * z);
	}

	public void add(double dx, double dy, double dz)
	{
		x += dx;
		y += dy;
		z += dz;
	}

	public void subtract(double dx, double dy, double dz)
	{
		x -= dx;
		y -= dy;
		z -= dz;
	}
}
