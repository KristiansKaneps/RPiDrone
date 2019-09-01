package lv.kaneps.rpidrone.math;

public class Vector3f
{
	public float x, y, z;

	public Vector3f(float x, float y, float z)
	{
		this.x = x;
		this.y = y;
		this.z = z;
	}

	public Vector3f(float[] xyzArr)
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

	public Vector3f()
	{
		this(0, 0, 0);
	}

	public double length()
	{
		return Math.sqrt(x * x + y * y + z * z);
	}

	public void add(float dx, float dy, float dz)
	{
		x += dx;
		y += dy;
		z += dz;
	}

	public void subtract(float dx, float dy, float dz)
	{
		x -= dx;
		y -= dy;
		z -= dz;
	}

	public float[] toArray()
	{
		return new float[]{x, y, z};
	}
}
