package lv.kaneps.rpidrone;

import java.io.PrintStream;
import java.util.Date;

public class Log
{
	public static PrintStream STREAM = System.out;

	private static final Date date = new Date();

	private static final String INFO = "[INFO]";
	private static final String WARN = "[WARNING]";
	private static final String ERR = "[ERROR]";
	private static final String FATAL = "[FATAL]";

	public static void i(Object info)
	{
		o(INFO, info);
	}

	public static void w(Object warning)
	{
		o(WARN, warning);
	}

	public static void e(Object error)
	{
		o(ERR, error);
	}

	public static void f(Object fatal)
	{
		o(FATAL, fatal);
	}

	private static void o(Object prefix, Object o)
	{
		STREAM.println(getDateAndTimePrefix() + ' ' + prefix.toString() + ' ' + o.toString());
	}

	private static String getDateAndTimePrefix()
	{
		return '[' + date.toString() + ']';
	}
}
