#include <Arduino.h>

// use only 1 core
#if CONFIG_FREERTOS_UNICORE
static const BaseType_t app_cpu = 0;
#else
static const BaseType_t app_cpu = 1;
#endif

// pins
const int ledPin = 2;

// task: blink led
void toggleLED(void *parameter) {
  while(1) {
    digitalWrite(ledPin, HIGH);
    vTaskDelay(500 / portTICK_PERIOD_MS);
    digitalWrite(ledPin, LOW);
    vTaskDelay(500 / portTICK_PERIOD_MS);
  }
}

void setup() {
  // init led pin
  pinMode(ledPin, OUTPUT);

  // create task
  // we want to run this task on just 1 core, so we use xTaskCreatePinnedToCore instead of xTaskCreate
  xTaskCreatePinnedToCore(
    toggleLED, // function to be called
    "Toggle LED", // name of the task
    1024, // stack size (bytes in esp32, words in FreeRTOS)
    NULL, // parameter to be passed to the function
    1, // task priorities (0 is the lowest priority __ and configMAX_PRIORITIES - 1 is the highest priority)
    NULL, // task handle (not used here)
    app_cpu // core to run the task on (0 or 1) __ run on one core for demo purposes (ESP32 only)
  );
}

void loop() {
// nothing to do here, the task will run independently
}