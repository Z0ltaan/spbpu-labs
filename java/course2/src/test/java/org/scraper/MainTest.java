package org.scraper;

import org.junit.jupiter.api.Test;
import org.junit.jupiter.api.extension.ExtendWith;
import org.mockito.MockedConstruction;
import org.mockito.junit.jupiter.MockitoExtension;

import java.io.ByteArrayOutputStream;
import java.io.PrintStream;
import java.util.concurrent.*;

import static org.junit.jupiter.api.Assertions.*;
import static org.mockito.ArgumentMatchers.*;
import static org.mockito.Mockito.*;

@ExtendWith(MockitoExtension.class)
public class MainTest {

  @Test
  public void testMainWithInsufficientArguments() {
    String[] args = {"1", "10"};

    PrintStream originalOut = System.out;
    ByteArrayOutputStream outContent = new ByteArrayOutputStream();
    System.setOut(new PrintStream(outContent));

    Main.main(args);

    System.setOut(originalOut);

    assertTrue(outContent.toString().contains("Error"));
  }

  @Test
  public void testMainWithInvalidThreshold() {
    String[] args = {"invalid", "10", "service1", "csv"};

    PrintStream originalOut = System.out;
    ByteArrayOutputStream outContent = new ByteArrayOutputStream();
    System.setOut(new PrintStream(outContent));

    Main.main(args);

    System.setOut(originalOut);

    assertTrue(outContent.toString().contains("Error"));
  }

  @Test
  public void testMainWithInvalidTimeout() {
    String[] args = {"2", "invalid", "service1", "csv"};

    PrintStream originalOut = System.out;
    ByteArrayOutputStream outContent = new ByteArrayOutputStream();
    System.setOut(new PrintStream(outContent));

    Main.main(args);

    System.setOut(originalOut);

    assertTrue(outContent.toString().contains("Error"));
  }

  @Test
  public void testMainWithGoodArgs() {
    String[] args = {"1", "2", "service1", "json"};

    try (MockedConstruction<DataWriter> dataWriterMock = mockConstruction(DataWriter.class);
         MockedConstruction<ScheduledThreadPoolExecutor> executorMock = mockConstruction(
                 ScheduledThreadPoolExecutor.class
         );
         MockedConstruction<ApiPoller> apiPollerMock = mockConstruction(ApiPoller.class)) {

      PrintStream originalOut = System.out;
      ByteArrayOutputStream outContent = new ByteArrayOutputStream();
      System.setOut(new PrintStream(outContent));

      Main.main(args);

      System.setOut(originalOut);

      assertFalse(outContent.toString().contains("Error"));

      DataWriter mockDataWriter = dataWriterMock.constructed().get(0);
      verify(mockDataWriter).close();

      ScheduledThreadPoolExecutor mockExecutor = executorMock.constructed().get(0);
      verify(mockExecutor).scheduleAtFixedRate(any(), eq(0L), eq(2L), eq(TimeUnit.SECONDS));

      assertEquals(1, apiPollerMock.constructed().size());
    }
  }
}