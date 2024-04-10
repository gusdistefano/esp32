#include "button_adc.h"
#include "esp_adc/adc_oneshot.h"
#include <esp_timer.h>
#include <esp_log.h>

#define DEAD_TIME_US 50000 // 50ms

#define ADC_CHANNEL CONFIG_ADC_CHANNEL

#define POLL_TIMEOUT_US (CONFIG_BUTTON_ADC_POLL_TIMEOUT * 1000)
#define POLL_TIMEOUT_TEST_US (CONFIG_BUTTON_ADC_POLL_TEST_TIMEOUT * 1000)
#define AUTOREPEAT_TIMEOUT_US (CONFIG_BUTTON_ADC_AUTOREPEAT_TIMEOUT * 1000)
#define AUTOREPEAT_INTERVAL_US (CONFIG_BUTTON_ADC_AUTOREPEAT_INTERVAL * 1000)
#define LONG_PRESS_TIMEOUT_US (CONFIG_BUTTON_ADC_LONG_PRESS_TIMEOUT * 1000)

static button_t *buttons[CONFIG_BUTTON_ADC_MAX] = {NULL};
static esp_timer_handle_t timer = NULL;

#define CHECK(x)                \
    do                          \
    {                           \
        esp_err_t __;           \
        if ((__ = x) != ESP_OK) \
            return __;          \
    } while (0)
#define CHECK_ARG(VAL)                  \
    do                                  \
    {                                   \
        if (!(VAL))                     \
            return ESP_ERR_INVALID_ARG; \
    } while (0)

//-------------ADC1 Init---------------//
adc_oneshot_unit_handle_t adc1_handle;
adc_oneshot_unit_init_cfg_t init_config = {
    .unit_id = ADC_UNIT_1,
    .ulp_mode = ADC_ULP_MODE_DISABLE,
};

//-------------ADC1 Config---------------//
adc_oneshot_chan_cfg_t config = {
    .bitwidth = ADC_BITWIDTH_DEFAULT,
    .atten = ADC_ATTEN_DB_11,
};

static const char *TAG = "Button_ADC";

static void poll_button(button_t *btn)
{
    if (btn->internal.state == BUTTON_PRESSED && btn->internal.pressed_time < DEAD_TIME_US)
    {
        // Dead time, ignore all
        btn->internal.pressed_time += POLL_TIMEOUT_US;
        return;
    }

    int adc_reading;

    adc_oneshot_read(adc1_handle, ADC_CHANNEL_6, &adc_reading);

    if (adc_reading > btn->lower_limit && adc_reading < btn->upper_limit)
    {
        // button is pressed

        if (btn->internal.state == BUTTON_RELEASED)
        {
            // pressing just started, reset pressing/repeating time and run callback
            btn->internal.state = BUTTON_PRESSED;
            btn->internal.pressed_time = 0;
            btn->internal.repeating_time = 0;
            btn->callback(btn, BUTTON_PRESSED);
            return;
        }
        // increment pressing time
        btn->internal.pressed_time += POLL_TIMEOUT_US;

        // check autorepeat
        if (btn->autorepeat)
        {
            // check autorepeat timeout
            if (btn->internal.pressed_time < AUTOREPEAT_TIMEOUT_US)
                return;
            // increment repeating time
            btn->internal.repeating_time += POLL_TIMEOUT_US;

            if (btn->internal.repeating_time >= AUTOREPEAT_INTERVAL_US)
            {
                // reset repeating time and run callback
                btn->internal.repeating_time = 0;
                btn->callback(btn, BUTTON_CLICKED);
            }
            return;
        }

        if (btn->internal.state == BUTTON_PRESSED && btn->internal.pressed_time >= LONG_PRESS_TIMEOUT_US)
        {
            // button pressed long time, change state and run callback
            btn->internal.state = BUTTON_PRESSED_LONG;
            btn->callback(btn, BUTTON_PRESSED_LONG);
        }
    }
    else if (btn->internal.state != BUTTON_RELEASED)
    {
        // button released
        bool clicked = btn->internal.state == BUTTON_PRESSED;
        btn->internal.state = BUTTON_RELEASED;
        btn->callback(btn, BUTTON_RELEASED);
        if (clicked)
            btn->callback(btn, BUTTON_CLICKED);
    }
}

static void poll(void *arg)
{

    for (size_t i = 0; i < CONFIG_BUTTON_ADC_MAX; i++)
        if (buttons[i] && buttons[i]->callback)
            poll_button(buttons[i]);
}

static void button_test(void *arg)
{
    int adc_reading;
    adc_oneshot_read(adc1_handle, ADC_CHANNEL, &adc_reading);
    ESP_LOGI(TAG, "%i", adc_reading);
};

////////////////////////////////////////////////////////////////////////////////

static const esp_timer_create_args_t timer_args = {
    .arg = NULL,
    .name = "poll_buttons",
    .dispatch_method = ESP_TIMER_TASK,
    .callback = poll,
};

static const esp_timer_create_args_t timer_args_test = {
    .arg = NULL,
    .name = "test",
    .dispatch_method = ESP_TIMER_TASK,
    .callback = button_test,
};
void init_adc(void)
{
    //-------------ADC1 Init---------------//

    ESP_ERROR_CHECK(adc_oneshot_new_unit(&init_config, &adc1_handle));

    //-------------ADC1 Config---------------//

    ESP_ERROR_CHECK(adc_oneshot_config_channel(adc1_handle, ADC_CHANNEL, &config));
}

esp_err_t button_init_test()
{
    ESP_LOGI(TAG, "TEST");
    CHECK(esp_timer_create(&timer_args_test, &timer));
    init_adc();
    CHECK(esp_timer_start_periodic(timer, POLL_TIMEOUT_US * 30));

    return ESP_OK;
}

esp_err_t button_init(button_t *btn)
{

    if (!timer && btn != NULL) // por primera vez|
    {
        ESP_LOGI(TAG, "NORMAL");
        CHECK(esp_timer_create(&timer_args, &timer));
        init_adc();
    }

    if (btn != NULL)
    {
        esp_timer_stop(timer);
        for (size_t i = 0; i < CONFIG_BUTTON_ADC_MAX; i++)
        {
            if (buttons[i] == btn)
                break;

            if (!buttons[i])
            {
                btn->internal.state = BUTTON_RELEASED;
                btn->internal.pressed_time = 0;
                btn->internal.repeating_time = 0;
                buttons[i] = btn;
                break;
            }
        }
    }

    CHECK(esp_timer_start_periodic(timer, POLL_TIMEOUT_US));

    return ESP_OK;
}

esp_err_t button_done(button_t *btn)
{
    CHECK_ARG(btn);

    esp_timer_stop(timer);

    esp_err_t res = ESP_ERR_INVALID_ARG;

    for (size_t i = 0; i < CONFIG_BUTTON_ADC_MAX; i++)
        if (buttons[i] == btn)
        {
            buttons[i] = NULL;
            res = ESP_OK;
            break;
        }

    CHECK(esp_timer_start_periodic(timer, POLL_TIMEOUT_US));
    return res;
}
