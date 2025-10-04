package org.scraper;

import java.io.IOException;
import java.util.concurrent.ScheduledExecutorService;
import java.util.concurrent.Semaphore;
import java.util.concurrent.ScheduledThreadPoolExecutor;
import java.util.concurrent.TimeUnit;

public class Main {
  public static void main(String[] args) {
    try {
      if (args.length < 4) {
        throw new IllegalArgumentException("Insufficient arguments");
      }

      int threshold = Integer.parseInt(args[0]);
      int timeout = Integer.parseInt(args[1]);
      String format = args[args.length - 1].toLowerCase();
      String[] services = args[2].split(" ");

      int initialDelay = 0;
      int lifeTime = 5;

      try (DataWriter dataWriter = new DataWriter(format);
          ScheduledExecutorService pollers = new ScheduledThreadPoolExecutor(services.length)) {
        Semaphore semaphore = new Semaphore(threshold);
        for (String api : services) {
          pollers.scheduleAtFixedRate(
              new ApiPoller(new ApiServiceModel(api), dataWriter, semaphore),
              initialDelay,
              timeout,
              TimeUnit.SECONDS);
        }

        pollers.awaitTermination(lifeTime, TimeUnit.SECONDS);
        pollers.shutdown();
      }
    } catch (IllegalArgumentException
        | InterruptedException
        | IOException
        | NullPointerException e) {
      System.err.println("Error: " + e.getMessage());
      e.printStackTrace();
    }
  }
}
