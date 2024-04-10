#include <stdio.h>
#include "button_adc.h"
#include <esp_log.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/timers.h"

static const char *TAG = "button_example";

static const char *states[] = {
    [BUTTON_PRESSED] = "pressed",
    [BUTTON_RELEASED] = "released",
    [BUTTON_CLICKED] = "clicked",
    [BUTTON_PRESSED_LONG] = "pressed long",
};

static button_t btn1, btn2, btn3, btn4;

static void on_button(button_t *btn, button_state_t state)
{
    ESP_LOGI(TAG, "%i button %s", btn->btn_number, states[state]);
}

void app_main(void)
{
    
    // Button 1
    btn1.btn_number = 1;
    btn1.lower_limit = 500;
    btn1.upper_limit = 700;
    btn1.autorepeat = false;
    btn1.callback = on_button;

    // Button 2
    btn2.btn_number = 2;
    btn2.lower_limit = 1350;
    btn2.upper_limit = 1400;
    btn2.autorepeat = false;
    btn2.callback = on_button;

    // Button 3
    btn3.btn_number = 3;
    btn3.lower_limit = 2090;
    btn3.upper_limit = 2190;
    btn3.autorepeat = false;
    btn3.callback = on_button;

    // Button 4
    btn4.btn_number = 4;
    btn4.lower_limit = 2900;
    btn4.upper_limit = 3000;
    btn4.autorepeat = false;
    btn4.callback = on_button;


    
    ESP_ERROR_CHECK(button_init(NULL));
    
   // ESP_ERROR_CHECK(button_init(&btn2));

   // ESP_ERROR_CHECK(button_init(&btn3));

   // ESP_ERROR_CHECK(button_init(&btn4));
}
