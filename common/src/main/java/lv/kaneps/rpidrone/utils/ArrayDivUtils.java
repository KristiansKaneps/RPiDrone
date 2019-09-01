package lv.kaneps.rpidrone.utils;

public final class ArrayDivUtils
{
	private ArrayDivUtils() {}

	/**
	 * Helper method for IMUSensor calibration
	 */
	public static int[] div(long[] array, int divisor)
	{
		int[] intArray = new int[array.length];
		for(int i = 0; i < intArray.length; i++)
			intArray[i] = (int) Math.floorDiv(array[i], (long) divisor);
		return intArray;
	}
}
