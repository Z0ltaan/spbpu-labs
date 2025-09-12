package org.scraper;

import org.junit.jupiter.api.Test;

import static org.junit.jupiter.api.Assertions.assertThrows;
import static org.junit.jupiter.api.Assertions.assertDoesNotThrow;
import static org.junit.jupiter.api.Assertions.assertEquals;

public class ApiServiceModelTest {
  @Test
  public void testConstructor() {
    assertThrows(IllegalArgumentException.class, () -> new ApiServiceModel(""));
    assertThrows(IllegalArgumentException.class, () -> new ApiServiceModel(null));
    assertDoesNotThrow(() -> new ApiServiceModel("someurl"));
  }

  @Test
  public void getUrl() {
    ApiServiceModel tmp = new ApiServiceModel("someurl");
    assertEquals("someurl", tmp.getUrl());
  }

}
