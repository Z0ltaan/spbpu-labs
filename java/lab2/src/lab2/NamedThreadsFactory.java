package lab2;

import java.util.concurrent.ThreadFactory;
import java.util.concurrent.atomic.AtomicInteger;

public class NamedThreadsFactory implements ThreadFactory {
  private final String prefix;
  private AtomicInteger counter;

  public NamedThreadsFactory(String prefix) {
    this.prefix = prefix;
    counter = new AtomicInteger(0);
  }

  @Override
  public Thread newThread(Runnable r) {
    return new Thread(r, prefix + counter.incrementAndGet());
  }
}
