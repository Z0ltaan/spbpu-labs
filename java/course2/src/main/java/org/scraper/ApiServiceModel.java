package org.scraper;

public class ApiServiceModel {
  private final String url_;

  ApiServiceModel(String url) throws IllegalArgumentException {
    if (url == null || url.isEmpty()) {
      throw new IllegalArgumentException("url is null or empty");
    }
    url_ = url;
  }

  public String getUrl() {
    return url_;
  }
}
