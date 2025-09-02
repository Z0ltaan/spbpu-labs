package lab2;

import java.util.concurrent.BlockingQueue;

public class QueueReader implements Runnable {
  private BlockingQueue<String> queue;

  public QueueReader(BlockingQueue<String> queue) {
    this.queue = queue;
  }

  public void readFromQueue() {
    System.out.println(
        Thread.currentThread().getName() + " has removed message: '" + queue.poll() + "'");
  }

  @Override
  public void run() {
    while (!Thread.currentThread().isInterrupted()) {
      this.readFromQueue();
    }
  }
}
