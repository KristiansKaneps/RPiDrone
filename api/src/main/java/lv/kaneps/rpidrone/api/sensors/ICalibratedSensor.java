package lv.kaneps.rpidrone.api.sensors;

public interface ICalibratedSensor<T, E extends ICalibratedSensor.ICalTypeEnum>
{
	/**
	 * Apply custom calibration data
	 */
	void applyCustomCal(T calData);

	/**
	 * Apply already measured calibration data (call calMeasure(...) to measure)
	 */
	default void applyMeasuredCal() { applyCustomCal(getMeasuredCal()); }

	/**
	 * @return applied calibration data (not the same as measured calibration data)
	 */
	T getAppliedCal();

	/**
	 * @return measured calibration data (not the same as applied calibration data)
	 */
	T getMeasuredCal();

	/**
	 * Manually set measured calibration data without the need of calMeasure(...)
	 */
	void setMeasuredCalData(T calData);

	/**
	 * Checks if this sensor is currently measuring samples for calibration data
	 */
	boolean isCalibrating();

	/**
	 * Checks if this sensor is calibrated
	 */
	boolean isCalibrated();

	/**
	 * @param type - calibration type
	 * @param num - sample count
	 * @param calData - calibration data delegate
	 * @return interface that can apply measured calibration data depending on the implementation
	 */
	IFutureApplyMeasuredCal calMeasure(E type, int num, IFutureData<T> calData);

	/**
	 * @param type - calibration type
	 * @param num - sample count
	 * @return interface that can apply measured calibration data depending on the implementation
	 */
	default IFutureApplyMeasuredCal calMeasure(E type, int num)
	{
		return this.calMeasure(type, num, calData -> {});
	}

	@FunctionalInterface
	interface IFutureData<T>
	{
		/**
		 * Called when measurements are finished
		 *
		 * @param calData - measured calibration data
		 */
		void dataReady(T calData);
	}

	@FunctionalInterface
	interface IFutureApplyMeasuredCal
	{
		/**
		 * Applies measured calibration data
		 */
		void futureApply();
	}

	/**
	 * Calibration type enums must implement this interface
	 */
	interface ICalTypeEnum {}
}
