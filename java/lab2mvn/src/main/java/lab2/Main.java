package lab2;

import java.util.concurrent.*;

public class Main {
  public static void main(String[] args) {
    if (args.length != 1) {
      System.err.println("Wrong number of arguments");
      return;
    }

    BlockingQueue<String> queue = new LinkedBlockingQueue<>();

    try {
      int threadsNum = Integer.parseInt(args[0]);

      ExecutorService Writers = Executors.newFixedThreadPool(threadsNum, new NamedThreadsFactory("Writer"));
      ExecutorService Readers = Executors.newFixedThreadPool(threadsNum, new NamedThreadsFactory("Reader"));

      for (int i = 0; i < threadsNum; ++i) {
        Writers.execute(new QueueWriter(queue));
        Readers.execute(new QueueReader(queue));
      }

      try {
        Thread.sleep(100);
      } finally {
        Writers.shutdownNow();
        Readers.shutdownNow();
      }
      System.out.println(queue.size());
    } catch (NullPointerException | IllegalArgumentException | InterruptedException e) {
      e.printStackTrace();
    }
  }
}
