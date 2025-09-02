package lab1;

public class Supervisor implements Runnable {
  final private AbstractProgram program;
  final private Thread programThread;

  public Supervisor(AbstractProgram program) throws NullPointerException {
    if (program == null)
      throw new NullPointerException();
    this.program = program;
    this.programThread = new Thread(program, "Abstract Program");
  }

  @Override
  public void run() {

    synchronized (this.program) {
      this.startAbstractProgram();

      try {
        while (!program.hasErrors()) {
          program.wait();
          System.out.println(Thread.currentThread().getName() + " detected " + program.getState());
          if (program.hasStopped()) {
            System.out.println("Restarting program...");
            program.setState(State.RUNNING);
          }
          program.notify();
        }
      } catch (InterruptedException e) {}

      this.stopAbstractProgram();
    }
  }

  public void startAbstractProgram() {
    System.out.println("Starting AbstractProgram");
    programThread.start();
  }

  public void stopAbstractProgram() {
    System.out.println("Stopping AbstractProgram");
    programThread.interrupt();
  }
}