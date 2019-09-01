package lv.kaneps.rpidrone.math;

public class Vector2i
{
	public int x, y;

	public Vector2i(int x, int y)
	{
		this.x = x;
		this.y = y;
	}

	public Vector2i(int[] xyArr)
	{
		if(xyArr.length < 2)
		{
			x = 0;
			y = 0;
		}

		x = xyArr[0];
		y = xyArr[1];
	}

	public Vector2i()
	{
		this(0, 0);
	}

	public double length()
	{
		return Math.sqrt(x * x + y * y);
	}

	public void add(int dx, int dy)
	{
		x += dx;
		y += dy;
	}

	public void subtract(int dx, int dy)
	{
		x -= dx;
		y -= dy;
	}

	public int[] toArray()
	{
		return new int[]{x, y};
	}
}
