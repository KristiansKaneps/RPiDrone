package lv.kaneps.rpidrone.math;

public class Vector2d
{
	public double x, y;

	public Vector2d(double x, double y)
	{
		this.x = x;
		this.y = y;
	}

	public Vector2d(double[] xyArr)
	{
		if(xyArr.length < 2)
		{
			x = 0;
			y = 0;
		}

		x = xyArr[0];
		y = xyArr[1];
	}

	public Vector2d()
	{
		this(0, 0);
	}

	public double length()
	{
		return Math.sqrt(x * x + y * y);
	}

	public void add(double dx, double dy)
	{
		x += dx;
		y += dy;
	}

	public void subtract(double dx, double dy)
	{
		x -= dx;
		y -= dy;
	}

	public double[] toArray()
	{
		return new double[]{x, y};
	}
}
