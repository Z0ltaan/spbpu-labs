package org.scraper;

import org.junit.jupiter.api.Test;
import org.junit.jupiter.api.extension.ExtendWith;
import org.mockito.MockedConstruction;
import org.mockito.junit.jupiter.MockitoExtension;

import java.io.ByteArrayOutputStream;
import java.io.PrintStream;

import static org.junit.jupiter.api.Assertions.assertTrue;
import static org.junit.jupiter.api.Assertions.assertFalse;
import static org.junit.jupiter.api.Assertions.assertEquals;
import static org.mockito.Mockito.mockConstruction;
import static org.mockito.Mockito.verify;
import static org.mockito.Mockito.atLeast;

@ExtendWith(MockitoExtension.class)
public class MainTest {

  @Test
  public void testMainWithInsufficientArguments() {
    String[] args = { "1", "10" };

    PrintStream originalOut = System.out;
    ByteArrayOutputStream outContent = new ByteArrayOutputStream();
    System.setErr(new PrintStream(outContent));

    Main.main(args);

    System.setErr(originalOut);

    assertTrue(outContent.toString().contains("Error"));
  }

  @Test
  public void testMainWithInvalidThreshold() {
    String[] args = { "invalid", "10", "service1", "csv" };

    PrintStream originalOut = System.out;
    ByteArrayOutputStream outContent = new ByteArrayOutputStream();
    System.setErr(new PrintStream(outContent));

    Main.main(args);

    System.setErr(originalOut);

    assertTrue(outContent.toString().contains("Error"));
  }

  @Test
  public void testMainWithInvalidTimeout() {
    String[] args = { "2", "invalid", "https://meowfacts.herokuapp.com/", "csv" };

    PrintStream originalOut = System.out;
    ByteArrayOutputStream outContent = new ByteArrayOutputStream();
    System.setErr(new PrintStream(outContent));

    Main.main(args);

    System.setErr(originalOut);

    assertTrue(outContent.toString().contains("Error"));
  }

  @Test
  public void testMainWithGoodArgs() {
    String[] args = { "1", "2", "https://meowfacts.herokuapp.com/", "json" };

    try (MockedConstruction<DataWriter> dataWriterMock = mockConstruction(DataWriter.class);
        MockedConstruction<ApiPoller> apiPollerMock = mockConstruction(ApiPoller.class)) {

      PrintStream originalOut = System.out;
      ByteArrayOutputStream outContent = new ByteArrayOutputStream();
      System.setErr(new PrintStream(outContent));

      Main.main(args);

      System.setErr(originalOut);

      assertFalse(outContent.toString().contains("Error"));

      DataWriter mockDataWriter = dataWriterMock.constructed().get(0);
      verify(mockDataWriter).close();

      assertEquals(1, apiPollerMock.constructed().size());
      ApiPoller apiPoller = apiPollerMock.constructed().get(0);
      verify(apiPoller, atLeast(1)).run();
    }
  }
}
