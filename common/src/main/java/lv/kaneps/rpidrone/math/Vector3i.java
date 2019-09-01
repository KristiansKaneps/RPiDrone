package lv.kaneps.rpidrone.math;

public class Vector3i
{
	public int x, y, z;

	public Vector3i(int x, int y, int z)
	{
		this.x = x;
		this.y = y;
		this.z = z;
	}

	public Vector3i(int[] xyzArr)
	{
		if(xyzArr.length < 3)
		{
			x = 0;
			y = 0;
			z = 0;
		}

		x = xyzArr[0];
		y = xyzArr[1];
		z = xyzArr[2];
	}

	public Vector3i()
	{
		this(0, 0, 0);
	}

	public double length()
	{
		return Math.sqrt(x * x + y * y + z * z);
	}

	public void add(int dx, int dy, int dz)
	{
		x += dx;
		y += dy;
		z += dz;
	}

	public void subtract(int dx, int dy, int dz)
	{
		x -= dx;
		y -= dy;
		z -= dz;
	}

	public int[] toArray()
	{
		return new int[]{x, y, z};
	}

	@Override
	public String toString()
	{
		return "x=" + x + ", y=" + y + ", z=" + z;
	}
}
