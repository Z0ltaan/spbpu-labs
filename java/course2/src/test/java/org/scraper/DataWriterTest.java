package org.scraper;

import org.junit.jupiter.api.AfterEach;
import org.junit.jupiter.api.Test;

import java.io.IOException;
import java.nio.file.Files;
import java.nio.file.Path;
import java.nio.file.Paths;

import static org.junit.jupiter.api.Assertions.assertTrue;
import static org.junit.jupiter.api.Assertions.assertDoesNotThrow;
import static org.junit.jupiter.api.Assertions.assertThrows;

public class DataWriterTest {
  private final String csvFileName = "api_data.csv";
  private final String jsonFileName = "api_data.json";

  @Test
  public void testConstructor() {
    assertThrows(IllegalArgumentException.class, () -> new DataWriter(null));
    assertThrows(IllegalArgumentException.class, () -> new DataWriter(""));
    assertThrows(IllegalArgumentException.class, () -> new DataWriter("wrongformat"));
    assertDoesNotThrow(() -> new DataWriter("json"));
    assertDoesNotThrow(() -> new DataWriter("csv"));
  }

  @AfterEach
  void getRidOfFiles() throws IOException {
    Files.deleteIfExists(Paths.get(csvFileName));
    Files.deleteIfExists(Paths.get(jsonFileName));
  }

  @Test
  void testWriteDataToCSV() {
    DataWriter csvWriter = null;
    try {
      csvWriter = new DataWriter("csv");
      csvWriter.writeData("testService", "test,data");
      String content = Files.readString(Path.of(csvFileName));
      assertTrue(content.contains("testService"));
      assertTrue(content.contains("test,data"));
    } catch (IOException e) {
      e.printStackTrace();
      System.err.println(e.getMessage());
    } finally {
      if (csvWriter != null) {
        csvWriter.close();
      }
    }
  }

  @Test
  void testWriteDataToJSON() {
    DataWriter jsonWriter = null;
    try {
      jsonWriter = new DataWriter("json");
      jsonWriter.writeData("testService", "{\"key\":\"value\"}");
      String content = Files.readString(Path.of(jsonFileName));
      assertTrue(content.contains("testService"));
      assertTrue(content.contains("\"key\":\"value\""));
    } catch (IOException e) {
      e.printStackTrace();
      System.err.println(e.getMessage());
    } finally {
      if (jsonWriter != null) {
        jsonWriter.close();
      }
    }
  }
}
