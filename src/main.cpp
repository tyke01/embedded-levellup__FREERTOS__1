#include <Arduino.h>

// use only 1 core
#if CONFIG_FREERTOS_UNICORE
static const BaseType_t app_cpu = 0;
#else
static const BaseType_t app_cpu = 1;
#endif


void testTask (void *parameter) {
  while (1) {
    int a = 1;
    int b[100];

    for (int i = 0; i < 100; i++) {
      b[i] = a + 1;
    }
    Serial.println(b[0]);

    // peint out remaining stack memory in words
    Serial.print("High water mark (words): ");
    Serial.println(uxTaskGetStackHighWaterMark(NULL));

    // Peint out number of free heap memory bytes before malloc
    Serial.print("Heap before malloc (bytes): ");
    Serial.println(xPortGetFreeHeapSize());

    int *ptr = (int *)pvPortMalloc(1024 * sizeof(int));

    // checking heap overflow by checking malloc output
    if(ptr == NULL) {
      Serial.println("Not enough heap:");
    } else {
      // Do something with the allocated memory so it's not optimized out by the compiler
      for (int i = 0; i < 1024; i++) {
        ptr[i] = 3;
      }
    }

    // Peint out number of free heap memory bytes after malloc
    Serial.print("Heap after malloc (bytes): ");
    Serial.println(xPortGetFreeHeapSize());


    // Free up our allocated memory
    vPortFree(ptr);
    vTaskDelay(100 / portTICK_PERIOD_MS);
  }
}

void setup() {
  Serial.begin(115200);

  // wait a moment to start
  vTaskDelay(1000 / portTICK_PERIOD_MS);
  Serial.println();

  Serial.println("--- FreeRTOS Memory Demo ---");

  // start the 1 only task

  xTaskCreatePinnedToCore(
    testTask,   // Task function
    "TestTask", // Name of the task
    1500,       // Stack size in words
    NULL,       // Task input parameter
    1,          // Priority of the task
    NULL,       // Task handle
    app_cpu      // Core where the task should run
  );

  vTaskDelete(NULL); // delete the setup and loop task
}

void loop () {
  // 
}