package lv.kaneps.rpidrone.math;

public final class Quaternion
{
	private double x; // x
	private double y; // y
	private double z; // z
	private double w; // scalar

	public Quaternion(final Quaternion q)
	{
		this(q.x, q.y, q.z, q.w);
	}

	public Quaternion(double x, double y, double z, double w)
	{
		this.x = x;
		this.y = y;
		this.z = z;
		this.w = w;
	}

	public void set(final Quaternion q)
	{
		this.x = q.x;
		this.y = q.y;
		this.z = q.z;
		this.w = q.w;
	}

	public Quaternion(Vector3d axis, double angle)
	{
		set(axis, angle);
	}

	public double norm()
	{
		return Math.sqrt(dot(this));
	}

	public double getW()
	{
		return w;
	}

	public double getX()
	{
		return x;
	}

	public double getY()
	{
		return y;
	}

	public double getZ()
	{
		return z;
	}

	/**
	 * @param axis  rotation axis, unit vector
	 * @param angle the rotation angle
	 * @return this
	 */
	public Quaternion set(Vector3d axis, double angle)
	{
		double s = Math.sin(angle / 2);
		w = Math.cos(angle / 2);
		x = axis.x * s;
		y = axis.y * s;
		z = axis.z * s;
		return this;
	}

	public Quaternion mulThis(Quaternion q)
	{
		double nw = w * q.w - x * q.x - y * q.y - z * q.z;
		double nx = w * q.x + x * q.w + y * q.z - z * q.y;
		double ny = w * q.y + y * q.w + z * q.x - x * q.z;
		z = w * q.z + z * q.w + x * q.y - y * q.x;
		w = nw;
		x = nx;
		y = ny;
		return this;
	}

	public Quaternion scaleThis(double scale)
	{
		if (scale != 1)
		{
			w *= scale;
			x *= scale;
			y *= scale;
			z *= scale;
		}
		return this;
	}

	public Quaternion divThis(double scale)
	{
		if (scale != 1)
		{
			w /= scale;
			x /= scale;
			y /= scale;
			z /= scale;
		}
		return this;
	}

	public double dot(Quaternion q)
	{
		return x * q.x + y * q.y + z * q.z + w * q.w;
	}

	public boolean equals(Quaternion q)
	{
		return x == q.x && y == q.y && z == q.z && w == q.w;
	}

	public Quaternion interpolateThis(Quaternion q, double t)
	{
		if (!equals(q))
		{
			double d = dot(q);
			double qx, qy, qz, qw;

			if (d < 0f)
			{
				qx = -q.x;
				qy = -q.y;
				qz = -q.z;
				qw = -q.w;
				d = -d;
			}
			else
			{
				qx = q.x;
				qy = q.y;
				qz = q.z;
				qw = q.w;
			}

			double f0, f1;

			if ((1 - d) > 0.1f)
			{
				double angle = Math.acos(d);
				double s = Math.sin(angle);
				double tAngle = t * angle;
				f0 = Math.sin(angle - tAngle) / s;
				f1 = Math.sin(tAngle) / s;
			}
			else
			{
				f0 = 1 - t;
				f1 = t;
			}

			x = f0 * x + f1 * qx;
			y = f0 * y + f1 * qy;
			z = f0 * z + f1 * qz;
			w = f0 * w + f1 * qw;
		}

		return this;
	}

	public Quaternion normalizeThis()
	{
		return divThis(norm());
	}

	public Quaternion interpolate(Quaternion q, double t)
	{
		return new Quaternion(this).interpolateThis(q, t);
	}

	/**
	 * Converts this Quaternion into a matrix, returning it as a float array.
	 */
	public float[] toMatrix()
	{
		float[] matrices = new float[16];
		toMatrix(matrices);
		return matrices;
	}

	/**
	 * Converts this Quaternion into a matrix, placing the values into the given array.
	 *
	 * @param matrices 16-length float array.
	 */
	public final void toMatrix(float[] matrices)
	{
		matrices[3] = 0.0f;
		matrices[7] = 0.0f;
		matrices[11] = 0.0f;
		matrices[12] = 0.0f;
		matrices[13] = 0.0f;
		matrices[14] = 0.0f;
		matrices[15] = 1.0f;

		matrices[0] = (float) (1.0f - (2.0f * ((y * y) + (z * z))));
		matrices[1] = (float) (2.0f * ((x * y) - (z * w)));
		matrices[2] = (float) (2.0f * ((x * z) + (y * w)));

		matrices[4] = (float) (2.0f * ((x * y) + (z * w)));
		matrices[5] = (float) (1.0f - (2.0f * ((x * x) + (z * z))));
		matrices[6] = (float) (2.0f * ((y * z) - (x * w)));

		matrices[8] = (float) (2.0f * ((x * z) - (y * w)));
		matrices[9] = (float) (2.0f * ((y * z) + (x * w)));
		matrices[10] = (float) (1.0f - (2.0f * ((x * x) + (y * y))));
	}
}