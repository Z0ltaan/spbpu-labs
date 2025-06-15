import lab1.AbstractProgram;
import lab1.Supervisor;

public class Main {
  public static void main(String[] args) {
    try {
      AbstractProgram program = new AbstractProgram();
      Supervisor supervisor = new Supervisor(program);
      Thread supervisorThread = new Thread(supervisor, "Supervisor");

      supervisorThread.start();
      supervisorThread.join();
    } catch (NullPointerException | InterruptedException e) {
      e.printStackTrace();
    }

  }
}
