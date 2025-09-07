package org.scraper;

public class ResponseInfoModel {
  private final String responseEntity;
  private final int responseCode;

  public ResponseInfoModel(String entity, int code) throws NullPointerException {
    if (entity == null) {
      throw new NullPointerException("null entity or url");
    }

    this.responseEntity = entity;
    this.responseCode = code;
  }

  public String getResponseEntity() {
    return responseEntity;
  }

  public int getResponseCode() {
    return responseCode;
  }
}
