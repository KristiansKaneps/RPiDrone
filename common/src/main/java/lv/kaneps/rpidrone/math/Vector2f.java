package lv.kaneps.rpidrone.math;

public class Vector2f
{
	public float x, y;

	public Vector2f(float x, float y)
	{
		this.x = x;
		this.y = y;
	}

	public Vector2f(float[] xyArr)
	{
		if(xyArr.length < 2)
		{
			x = 0;
			y = 0;
		}

		x = xyArr[0];
		y = xyArr[1];
	}

	public Vector2f()
	{
		this(0, 0);
	}

	public double length()
	{
		return Math.sqrt(x * x + y * y);
	}

	public void add(float dx, float dy)
	{
		x += dx;
		y += dy;
	}

	public void subtract(float dx, float dy)
	{
		x -= dx;
		y -= dy;
	}

	public float[] toArray()
	{
		return new float[]{x, y};
	}
}
