#include <stdio.h>
#include <esp_log.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/timers.h"
#include "buzzer.h"

static const char *TAG = "buzzer_example";


void app_main(void)
{
    ESP_LOGI(TAG, "Initializing the buzzer ...");
    init_buzzer();
 
    buzzer_alarm(50, 1000, 3);

    test_buzzer_alarm();

}