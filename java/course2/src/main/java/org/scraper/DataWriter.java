package org.scraper;

import java.io.FileWriter;
import java.io.IOException;
import java.time.LocalDateTime;
import java.util.concurrent.locks.ReentrantLock;

public class DataWriter {
  private final String format;
  private final ReentrantLock lock;
  private FileWriter fileWriter;
  private boolean isFirstEntry = true;

  public DataWriter(String format) throws IllegalArgumentException, IOException {
    if (format == null || format.isEmpty() || (!format.equals("csv") && !format.equals("json"))) {
      throw new IllegalArgumentException("format is null or empty or wrong");
    }
    this.format = format;
    initializeFile();
    this.lock = new ReentrantLock();
  }

  private void initializeFile() throws IOException {
      String fileName = "api_data." + format;
      fileWriter = new FileWriter(fileName, false);
      if (format.equals("csv")) {
        fileWriter.write("timestamp;service;data\n");
      } else if (format.equals("json")) {
        fileWriter.write("[\n");
      }
//      } else if (format.equals("json")) {
//        // Если файл уже существует, читаем его и проверяем структуру
//        String content = Files.readString(path).trim();
//        if (content.endsWith("]")) {
//          // Удаляем закрывающую скобку для добавления новых данных
//          content = content.substring(0, content.length() - 1);
//          if (!content.endsWith("[\n") && !content.endsWith("[")) {
//            content += ",";
//          }
//          Files.writeString(path, content);
//          isFirstEntry = false;
//        }
//      }
  }

  public void writeData(String serviceName, String data) {
    lock.lock();
    try {
      String timestamp = LocalDateTime.now().toString();
      String formattedData;

      if (format.equals("csv")) {
        formattedData = timestamp + ";" + serviceName + ";" + data;
      } else {
        String jsonEntry = "{\"timestamp\":\"" + timestamp + "\",\"service\":\""
                + serviceName + "\",\"data\":" + data + "}" ;

        if (isFirstEntry) {
          formattedData = jsonEntry + "\n";
          isFirstEntry = false;
        } else {
          formattedData = ",\n" + jsonEntry;
        }
      }

      fileWriter.write(formattedData);
      fileWriter.flush();
    } catch (IOException e) {
      System.err.println("Error writing data: " + e.getMessage());
    } finally {
      lock.unlock();
    }
  }

  public void close() {
    lock.lock();
    try {
      if (format.equals("json")) {
        fileWriter.write("\n]\n");
      }
      fileWriter.close();
    } catch (IOException e) {
      System.err.println("Error closing file writer: " + e.getMessage());
    } finally {
      lock.unlock();
    }
  }
}