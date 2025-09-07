package org.scraper;

import static org.junit.jupiter.api.Assertions.assertEquals;
import static org.junit.jupiter.api.Assertions.assertNotNull;
import static org.junit.jupiter.api.Assertions.assertThrows;
import org.junit.jupiter.api.Test;

public class ResponseInfoModelTest {
  @Test
  public void testConstructorAndGetters() {
    String entity = new String("");
    int code = 0;
    assertThrows(NullPointerException.class, () -> new ResponseInfoModel(null, 0));
    ResponseInfoModel info = null;
    info = new ResponseInfoModel(entity, code);
    assertNotNull(info);

    assertEquals(info.getResponseEntity(), entity);
    assertEquals(info.getResponseCode(), code);
  }
}
