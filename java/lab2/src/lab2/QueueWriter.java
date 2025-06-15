package lab2;

import java.util.concurrent.BlockingQueue;

public class QueueWriter implements Runnable {
  private BlockingQueue<String> queue;

  public QueueWriter(BlockingQueue<String> queue) {
    this.queue = queue;
  }

  public void writeInQueue(String message) {
    queue.add(message);
    System.out.println(message);
  }

  @Override
  public void run() {
    while (!Thread.currentThread().isInterrupted()) {
      this.writeInQueue(Thread.currentThread().getName() + " has written in queue");
    }
  }
}
