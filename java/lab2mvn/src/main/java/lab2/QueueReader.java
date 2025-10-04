package lab2;

import java.util.concurrent.BlockingQueue;

public class QueueReader implements Runnable {
  private BlockingQueue<String> queue;

  public QueueReader(BlockingQueue<String> queue) {
    this.queue = queue;
  }

  public void readFromQueue() throws InterruptedException {
    System.out.println(Thread.currentThread().getName()
        + " has removed message: '" + queue.take() + "'");
  }

  @Override
  public void run() {
    while (!Thread.currentThread().isInterrupted()) {
      try {
        this.readFromQueue();
      } catch (InterruptedException e) {
        System.err.println(e.getMessage());
      }
    }
  }
}
