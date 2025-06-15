package org.scraper;

import org.apache.http.client.methods.HttpGet;
import org.apache.http.impl.client.CloseableHttpClient;
import org.apache.http.impl.client.HttpClientBuilder;
import org.apache.http.util.EntityUtils;

import java.io.IOException;
import java.util.concurrent.Semaphore;

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
      pollService();
    } catch (InterruptedException e) {
      System.err.println("Error polling service " + service_.getUrl() + ": " + e.getMessage());
    } finally {
      semaphore.release();
    }
  }

  private void pollService(){
    try (CloseableHttpClient client = HttpClientBuilder.create().build()) {
      client.execute(new HttpGet(service_.getUrl()),
              response -> {
                if (response.getStatusLine().getStatusCode() == 200) {
                  dataWriter.writeData(service_.getUrl(), EntityUtils.toString(response.getEntity()));
                } else {
                  System.err.println("Error polling " + service_.getUrl() +
                          ": HTTP " + response.getStatusLine().getStatusCode());
                }
                return null;
              }
      );
    } catch (IOException e) {
      System.err.println("Error polling " + service_.getUrl() + ": " + e.getMessage());
    }
  }
}