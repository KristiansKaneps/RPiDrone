package lv.kaneps.rpidrone;

import org.jetbrains.annotations.NotNull;

import java.util.Scanner;

public final class Main
{
	private static final Scanner s = new Scanner(System.in);
	private static volatile boolean consoleRunning = true;
	private static volatile CommandHandler h;
	private static final MainThreadAction DEFAULT_ACTION = () -> {};

	public static void main(String[] args)
	{
		final Drone drone = new Drone();
		h = new CommandHandler(drone);
		drone.init();
		drone.start();

		new Thread(() -> {
			while(consoleRunning)
				h.execute(s.nextLine().toLowerCase()).run();
		}, "Console").start();
	}

	private static final class CommandHandler
	{
		private final Drone drone;

		private CommandHandler(Drone drone)
		{
			this.drone = drone;
		}

		@NotNull
		private final MainThreadAction execute(final String cmd)
		{
			switch(cmd)
			{
				default: return DEFAULT_ACTION;
			}
		}
	}

	private interface MainThreadAction extends Runnable {}
}
