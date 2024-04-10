#include <stdio.h>
#include <esp_log.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/timers.h"

#include "button_adc_lvgl.h"

static const char *TAG = "button_example";

void app_main(void)
{

    button_adc_config_t btn1 = {
        .button_index = 1,
        .lower_limit = 400,
        .upper_limit = 800};
    button_adc_config_t btn2 = {
        .button_index = 2,
        .lower_limit = 1300,
        .upper_limit = 1500};
    button_adc_config_t btn3 = {
        .button_index = 3,
        .lower_limit = 1900,
        .upper_limit = 2200};
    button_adc_config_t btn4 = {
        .button_index = 4,
        .lower_limit = 2600,
        .upper_limit = 3000};

    button_adc_init(&btn1);
    button_adc_init(&btn2);
    button_adc_init(&btn3);
    button_adc_init(&btn4);

    while (1)
    {
        ESP_LOGI(TAG, "button_id = %i", button_adc_get_pressed_id());
        vTaskDelay(pdMS_TO_TICKS(50));
    }

    /*In the case of using the 'button_adc_init_test()' testing,
    the buttons should not be initialized; it only serves
    to obtain the values ​​of the buttons.*/

    // button_adc_init_test()

    // Check the configuration parameters via 'idf.py menuconfig'
}
