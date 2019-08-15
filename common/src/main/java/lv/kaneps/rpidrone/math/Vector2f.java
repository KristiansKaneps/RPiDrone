package lv.kaneps.rpidrone.math;

public class Vector2f
{
	public float x, y;

	public Vector2f(float x, float y)
	{
		this.x = x;
		this.y = y;
	}

	public Vector2f()
	{
		this(0, 0);
	}

	public float length()
	{
		return (float) Math.sqrt(x * x + y * y);
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
}
