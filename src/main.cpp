#include <Arduino.h>

// use only 1 core
#if CONFIG_FREERTOS_UNICORE
static const BaseType_t app_cpu = 0;
#else
static const BaseType_t app_cpu = 1;
#endif

// const char msg[] = "Barkadeer brig Arr booty rum";
const int ledPin = 23;

// task handles
// static TaskHandle_t task_1 = NULL;
// static TaskHandle_t task_2 = NULL;

static TaskHandle_t read_serial = NULL;
static TaskHandle_t blink_led = NULL;

// ---------------- Tasks ---------------- //

void readSerial(void *parameter) {
  while (1) {
    if (Serial.available() > 0) {
      vTaskSuspend(blink_led);
      String inputStr = Serial.readString();
      inputStr.trim();
      int input = inputStr.toInt();
      Serial.println("Received input: " + String(input));
      digitalWrite(ledPin, HIGH);
      vTaskDelay(input / portTICK_PERIOD_MS);
      digitalWrite(ledPin, LOW);
      vTaskDelay(input / portTICK_PERIOD_MS);
      digitalWrite(ledPin, HIGH);
      vTaskDelay(input / portTICK_PERIOD_MS);
      digitalWrite(ledPin, LOW);
      vTaskDelay(input / portTICK_PERIOD_MS);
      digitalWrite(ledPin, HIGH);
      vTaskDelay(input / portTICK_PERIOD_MS);
      digitalWrite(ledPin, LOW);
      vTaskDelay(input / portTICK_PERIOD_MS);
      vTaskResume(blink_led);
    }
    vTaskDelay(100 / portTICK_PERIOD_MS);
  }
}

void blinkLed (void *parameter) {

  while (1) {
    digitalWrite(ledPin, HIGH);
    vTaskDelay(300 / portTICK_PERIOD_MS);
    digitalWrite(ledPin, LOW);
    vTaskDelay(300 / portTICK_PERIOD_MS);
  }
}


// ---------------- Main (runss as it's own task with priority 1 on core 1)
// ---------------- //

void setup() {
  
  Serial.begin(115200);
  pinMode(ledPin, OUTPUT);
  // create task 1
  xTaskCreatePinnedToCore(readSerial, // function that implements the task
                          "Task 1",   // name of the task
                          1024,       // stack size in words
                          NULL,       // task input parameter
                          2,          // priority of the task
                          &read_serial,    // task handle
                          app_cpu     // core where the task should run
  );

  // create task 2
  xTaskCreatePinnedToCore(blinkLed, // function that implements the task
                          "Task 2",   // name of the task
                          1024,       // stack size in words
                          NULL,       // task input parameter
                          1,          // priority of the task
                          &blink_led,    // task handle
                          app_cpu     // core where the task should run
  );
}

void loop() {

  // // Suspend the higher priority task for some intervals
  // for (int i = 0; i < 3; i++) {
  //   vTaskSuspend(blink_led);

  //   vTaskDelay(2000 / portTICK_PERIOD_MS);
  //   vTaskResume(task_2);
  //   vTaskDelay(2000 / portTICK_PERIOD_MS);
  // }

  // // Delete the lower priority task
  // if (task_1 != NULL) {
  //   vTaskDelete(task_1);
  //   task_1 = NULL;
  // }

}