package org.scraper;

import java.io.IOException;
import java.util.concurrent.*;

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
      DataWriter dataWriter = new DataWriter(format);

      ScheduledExecutorService pollers = new ScheduledThreadPoolExecutor(services.length);
      Semaphore semaphore = new Semaphore(threshold);
      for (String api : services) {
        pollers.scheduleAtFixedRate(
            new ApiPoller(new ApiServiceModel(api), dataWriter, semaphore),
            0,
            timeout,
            TimeUnit.SECONDS);
      }

      pollers.awaitTermination(10, TimeUnit.SECONDS);
      pollers.shutdownNow();
      dataWriter.close();
    } catch (IllegalArgumentException
        | InterruptedException
        | IOException
        | NullPointerException e) {
      System.out.println("Error: " + e.getMessage());
      // e.printStackTrace();
    }
  }
}
