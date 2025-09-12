package lab1;

import java.util.Random;

public class AbstractProgram implements Runnable {
  public State state;
  private final Random randomStateProducer;

  public AbstractProgram() {
    this.setState(State.UNKNOWN);
    this.randomStateProducer = new Random();
  }

  public synchronized boolean hasErrors() {
    return this.getState() == State.FATAL_ERROR;
  }

  public synchronized boolean hasStopped() {
    return this.getState() == State.STOPPING;
  }

  public synchronized void setState(State state) {
    System.out.println(Thread.currentThread().getName() + " set " + state.toString());
    this.state = state;
  }

  public synchronized void setRandomState() {
    this.setState(State.values()[randomStateProducer.nextInt(State.values().length)]);
  }

  public synchronized State getState() {
    return state;
  }

  @Override
  public void run() {
    Thread daemonSetter = new Thread(() -> {
      synchronized (AbstractProgram.this) {
        while (!AbstractProgram.this.hasErrors()) {
          try {
            setRandomState();
            notify();
            wait();
          } catch (InterruptedException e) {
            break;
          }
        }
      }
    }, "DAemon");

    daemonSetter.setDaemon(true);
    daemonSetter.start();

    while (!Thread.currentThread().isInterrupted()) {
    }
  }
}
