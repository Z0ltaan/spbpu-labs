package org.scraper;

import org.junit.jupiter.api.Test;
import org.junit.jupiter.api.extension.ExtendWith;
import org.mockito.Mock;
import org.mockito.junit.jupiter.MockitoExtension;

import java.util.concurrent.Semaphore;

import static org.junit.jupiter.api.Assertions.assertThrows;
import static org.junit.jupiter.api.Assertions.assertNotNull;
import static org.mockito.Mockito.*;

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
    assertThrows(NullPointerException.class, () -> new ApiPoller(mockService, mockDataWriter, null));
    assertThrows(NullPointerException.class, () -> new ApiPoller(null, mockDataWriter, mockSemaphore));
    assertThrows(NullPointerException.class, () -> new ApiPoller(mockService, null, mockSemaphore));
    ApiPoller apiPoller = null;
    apiPoller = new ApiPoller(mockService, mockDataWriter, mockSemaphore);
    assertNotNull(apiPoller);
  }

  @Test
  void testRunWithGoodApi() throws InterruptedException {
    when(mockService.getUrl()).thenReturn("https://meowfacts.herokuapp.com/");
    doNothing().when(mockDataWriter).writeData(anyString(), anyString());
    doNothing().when(mockSemaphore).acquire();
    doNothing().when(mockSemaphore).release();
    ApiPoller apiPoller = new ApiPoller(mockService, mockDataWriter, mockSemaphore);
    Thread thread = new Thread(apiPoller);
    thread.start();
    Thread.sleep(2000);
    thread.interrupt();
    thread.join();
    verify(mockService, atLeastOnce()).getUrl();
    verify(mockDataWriter, atLeastOnce()).writeData(anyString(), anyString());
  }
}
