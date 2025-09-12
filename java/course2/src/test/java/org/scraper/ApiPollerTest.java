package org.scraper;

import static org.junit.jupiter.api.Assertions.assertDoesNotThrow;
import static org.junit.jupiter.api.Assertions.assertNotNull;
import static org.junit.jupiter.api.Assertions.assertThrows;
import static org.mockito.Mockito.doNothing;
import static org.mockito.Mockito.when;
import static org.mockito.Mockito.atLeastOnce;
import static org.mockito.Mockito.verify;
import static org.mockito.Mockito.anyString;

import java.util.concurrent.Semaphore;
import org.junit.jupiter.api.Test;
import org.junit.jupiter.api.extension.ExtendWith;
import org.mockito.Mock;
import org.mockito.junit.jupiter.MockitoExtension;
import static org.junit.jupiter.api.Assertions.assertDoesNotThrow;

@ExtendWith(MockitoExtension.class)
class ApiPollerTest {
  @Mock
  private ApiServiceModel mockService;

  @Mock
  private DataWriter mockDataWriter;

  @Mock
  private Semaphore mockSemaphore;

  @Test
  public void testConstructor() {
    assertThrows(
        NullPointerException.class, () -> new ApiPoller(mockService, mockDataWriter, null));
    assertThrows(
        NullPointerException.class, () -> new ApiPoller(null, mockDataWriter, mockSemaphore));
    assertThrows(NullPointerException.class, () -> new ApiPoller(mockService, null, mockSemaphore));
    assertDoesNotThrow(() -> new ApiPoller(mockService, mockDataWriter, mockSemaphore));
  }

  @Test
  void testRunWithGoodApi() throws InterruptedException {
    when(mockService.getUrl()).thenReturn("https://meowfacts.herokuapp.com/");
    doNothing().when(mockDataWriter).writeData(anyString(), anyString());
    doNothing().when(mockSemaphore).acquire();
    doNothing().when(mockSemaphore).release();
    ApiPoller apiPoller = new ApiPoller(mockService, mockDataWriter,
        mockSemaphore);
    apiPoller.run();
    verify(mockService, atLeastOnce()).getUrl();
    verify(mockDataWriter, atLeastOnce()).writeData(anyString(), anyString());
  }
}
