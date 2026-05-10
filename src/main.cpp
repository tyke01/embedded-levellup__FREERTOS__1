#include <Arduino.h>

// use only 1 core
#if CONFIG_FREERTOS_UNICORE
static const BaseType_t app_cpu = 0;
#else
static const BaseType_t app_cpu = 1;
#endif

const char msg[] = "Barkadeer brig Arr booty rum";

// task handles
static TaskHandle_t task_1 = NULL;
static TaskHandle_t task_2 = NULL;

// ---------------- Tasks ---------------- //

void startTask1(void *parameter) {

  // Count number of characteds in string
  int msg_len = strlen(msg);

  while (1) {
    Serial.println('*');
    for (int i = 0; i < msg_len; i++) {
      Serial.print(msg[i]);
    }
    Serial.println();
    vTaskDelay(1000 / portTICK_PERIOD_MS);
  }
}

void startTask2(void *parameter) {
  while (1) {
    Serial.println("*");
    vTaskDelay(100 / portTICK_PERIOD_MS);
  }
}

// ---------------- Main (runss as it's own task with priority 1 on core 1)
// ---------------- //

void setup() {
  Serial.begin(115200);

  vTaskDelay(1000 / portTICK_PERIOD_MS);
  Serial.println();
  Serial.println("FreeRTOS task demo...");

  // print self priority
  Serial.print("Setup and loop running on core");
  Serial.println(xPortGetCoreID());
  Serial.print("With priority ");
  Serial.println(uxTaskPriorityGet(NULL));

  // create task 1
  xTaskCreatePinnedToCore(startTask1, // function that implements the task
                          "Task 1",   // name of the task
                          1024,       // stack size in words
                          NULL,       // task input parameter
                          1,          // priority of the task
                          &task_1,    // task handle
                          app_cpu     // core where the task should run
  );

  // create task 2
  xTaskCreatePinnedToCore(startTask2, // function that implements the task
                          "Task 2",   // name of the task
                          1024,       // stack size in words
                          NULL,       // task input parameter
                          2,          // priority of the task
                          &task_2,    // task handle
                          app_cpu     // core where the task should run
  );
}

void loop() {

  // Suspend the higher priority task for some intervals
  for (int i = 0; i < 3; i++) {
    vTaskSuspend(task_2);

    vTaskDelay(2000 / portTICK_PERIOD_MS);
    vTaskResume(task_2);
    vTaskDelay(2000 / portTICK_PERIOD_MS);
  }

  // Delete the lower priority task
  if (task_1 != NULL) {
    vTaskDelete(task_1);
    task_1 = NULL;
  }
}