import java.util.concurrent.*;

import lab2.NamedThreadsFactory;
import lab2.QueueReader;
import lab2.QueueWriter;

public class Main {
  public static void main(String[] args) {
    if (args.length != 1) {
      System.out.println("Wrong number of arguments");
      return;
    }

    BlockingQueue< String > queue = new LinkedBlockingQueue<>();

    int threadsNum = Integer.parseInt(args[0]);

    try {
      ExecutorService Writers = Executors.newFixedThreadPool(threadsNum, new NamedThreadsFactory("Writer"));
      ExecutorService Readers = Executors.newFixedThreadPool(threadsNum, new NamedThreadsFactory("Reader"));

      for (int i = 0; i < threadsNum; ++i) {
        Writers.execute(new QueueWriter(queue));
      }
      for (int i = 0; i < threadsNum; ++i) {
        Readers.execute(new QueueReader(queue));
      }

      try {
        Thread.sleep(4000);
      } catch (InterruptedException e) {}

      Writers.shutdownNow();
      Readers.shutdownNow();
    } catch (NullPointerException | IllegalArgumentException e) {
      System.out.println("bad");
    }
  }
}
