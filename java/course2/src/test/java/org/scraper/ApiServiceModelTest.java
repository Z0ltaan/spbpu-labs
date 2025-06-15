package org.scraper;

import org.junit.jupiter.api.Test;

import static org.junit.jupiter.api.Assertions.assertThrows;
import static org.junit.jupiter.api.Assertions.assertNotNull;
import static org.junit.jupiter.api.Assertions.assertEquals;

public class ApiServiceModelTest {
  @Test
  public void testConstructor() {
    assertThrows(IllegalArgumentException.class, () -> new ApiServiceModel(""));
    assertThrows(IllegalArgumentException.class, () -> new ApiServiceModel(null));
    ApiServiceModel tmp = new ApiServiceModel("someurl");
    assertNotNull(tmp);
  }

  @Test
  public void getUrl() {
    ApiServiceModel tmp = new ApiServiceModel("someurl");
    assertEquals("someurl", tmp.getUrl());
  }

}
