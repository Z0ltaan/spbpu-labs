package org.scraper;

import java.io.IOException;
import java.util.concurrent.Semaphore;
import org.apache.http.client.methods.HttpGet;
import org.apache.http.impl.client.CloseableHttpClient;
import org.apache.http.impl.client.HttpClientBuilder;
import org.apache.http.util.EntityUtils;

public class ApiPoller implements Runnable {
  private final ApiServiceModel service_;
  private final DataWriter dataWriter;
  private final Semaphore semaphore;

  public ApiPoller(ApiServiceModel service, DataWriter dataWriter, Semaphore sem) throws NullPointerException {
    if (service == null || dataWriter == null || sem == null) {
      throw new NullPointerException("null api service or null data writer or semaphore null");
    }
    this.service_ = service;
    this.dataWriter = dataWriter;
    this.semaphore = sem;
  }

  @Override
  public void run() {
    try {
      semaphore.acquire();
      ResponseInfoModel info = pollService();
      if (info.getResponseCode() != 200) {
        throw new IOException(
            "Error polling "
                + service_.getUrl()
                + ": HTTP "
                + info.getResponseCode());
      }
      writeData(info);
    } catch (InterruptedException | IOException e) {
      System.err.println("Poll error: " + e.getMessage());
    } finally {
      semaphore.release();
    }
  }

  private ResponseInfoModel pollService() throws IOException {
    var dataWrapper = new Object() {
      String responseData = "";
      int responseCode = 0;
    };

    try (CloseableHttpClient client = HttpClientBuilder.create().build()) {
      client.execute(
          new HttpGet(service_.getUrl()),
          response -> {
            dataWrapper.responseCode = response.getStatusLine().getStatusCode();
            dataWrapper.responseData = EntityUtils.toString(response.getEntity());
            return null;
          });
    }

    return new ResponseInfoModel(dataWrapper.responseData, dataWrapper.responseCode);
  }

  private void writeData(ResponseInfoModel info) {
    dataWriter.writeData(service_.getUrl(), info.getResponseEntity());
  }
}
