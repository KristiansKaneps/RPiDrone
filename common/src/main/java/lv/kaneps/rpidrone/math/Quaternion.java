package lv.kaneps.rpidrone.math;

public class Quaternion
{
	public float scalar, x, y, z;

	public Quaternion(float scalar, float x, float y, float z)
	{
		this.scalar = scalar;
		this.x = x;
		this.y = y;
		this.z = z;
	}

	public Quaternion()
	{
		this(0.0f, 0.0f, 0.0f, 0.0f);
	}
}