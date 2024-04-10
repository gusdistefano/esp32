#include "button_adc_lvgl.h"
#include "esp_adc/adc_oneshot.h"
#include <esp_timer.h>
#include <esp_log.h>

#ifdef CONFIG_ADC_UNIT_1
#define ADC_UNIT ADC_UNIT_1
#else
#define ADC_UNIT ADC_UNIT_2
#endif

#define ADC_CHANNEL CONFIG_ADC_CHANNEL
#define BUTTON_ADC_MAX CONFIG_BUTTON_ADC_MAX
#define POLL_TIMEOUT_US (CONFIG_BUTTON_ADC_POLL_TIMEOUT * 1000)

#define CHECK(x) do { esp_err_t __; if ((__ = x) != ESP_OK) return __; } while (0)
#define CHECK_ARG(VAL) do { if (!(VAL)) return ESP_ERR_INVALID_ARG; } while (0)

static button_adc_config_t *buttons[BUTTON_ADC_MAX] = {NULL};
static esp_timer_handle_t timer = NULL;

static const char *TAG = "Button ADC for LVGL";

//-------------ADC Init---------------//
adc_oneshot_unit_handle_t adc_handle;
adc_oneshot_unit_init_cfg_t init_config = {
    .unit_id = ADC_UNIT,
    .ulp_mode = ADC_ULP_MODE_DISABLE,
};

//-------------ADC Config---------------//
adc_oneshot_chan_cfg_t config = {
    .bitwidth = ADC_BITWIDTH_DEFAULT,
    .atten = ADC_ATTEN_DB_11,
};

/////////////////  INIT ADC   /////////////////////////////////////
static void init_adc(void)
{
    //-------------ADC Init---------------//
    ESP_ERROR_CHECK(adc_oneshot_new_unit(&init_config, &adc_handle));
    //-------------ADC Config------------//
    ESP_ERROR_CHECK(adc_oneshot_config_channel(adc_handle, ADC_CHANNEL, &config));
}

///////////////////// POLL BUTTONS   ///////////////////////////////////
static void poll_buttons(void *arg)
{
    static int adc_value = 0;
    adc_oneshot_read(adc_handle, ADC_CHANNEL, &adc_value);

    for (uint8_t i = 0; i < BUTTON_ADC_MAX; i++)
    {
        if (buttons[i])
        {
            if (adc_value > buttons[i]->lower_limit && adc_value < buttons[i]->upper_limit)
                buttons[i]->state = BUTTON_PRESSED;
            else
                buttons[i]->state = BUTTON_RELEASED;
        }
    }
}

//////////////// IS_PRESSED /////////////////////
bool button_adc_is_pressed(button_adc_config_t *btn)
{
    if (btn->state == BUTTON_PRESSED)
        return true;
    else
        return false;
};

//////////////// GET_ID /////////////////////
int8_t button_adc_get_pressed_id(void)
{
    for (uint8_t i = 0; i < BUTTON_ADC_MAX; i++)
    {
        if (buttons[i]->state == BUTTON_PRESSED)
        {
            return buttons[i]->button_index;
        }
    }
    return -1;
}

///////////// INIT / DEINIT ZONE /////////////////
static const esp_timer_create_args_t timer_args = {
    .arg = NULL,                       //!< Argument to pass to the callback
    .name = "poll_buttons",            //!< Timer name, used in esp_timer_dump function
    .dispatch_method = ESP_TIMER_TASK, //!< Call the callback from task or from ISR
    .callback = poll_buttons,          //!< Function to call when timer expires
};

esp_err_t button_adc_init(button_adc_config_t *btn)
{
    CHECK_ARG(btn);
    if (timer == NULL) // first time , init timer and adc
    {
        ESP_LOGI(TAG, "Starting Timer . . .");
        CHECK(esp_timer_create(&timer_args, &timer));
        ESP_LOGI(TAG, "Starting ADC . . .");
        init_adc();
    }

    if (btn != NULL)
    {
        esp_timer_stop(timer);
        for (u_int8_t i = 0; i < BUTTON_ADC_MAX; i++)
        {
            if (buttons[i] == btn)
                break;

            if (!buttons[i])
            {
                btn->state = BUTTON_RELEASED;
                buttons[i] = btn;
                ESP_LOGI(TAG, "Button added= %i press--VAL low=%i  Up=%i",
                         buttons[i]->button_index,
                         buttons[i]->lower_limit,
                         buttons[i]->upper_limit);
                break;
            }
        }
    }

    CHECK(esp_timer_start_periodic(timer, POLL_TIMEOUT_US));
    return ESP_OK;
};

esp_err_t button_adc_deinit(button_adc_config_t *btn)
{
    CHECK_ARG(btn);
    esp_timer_stop(timer);
    esp_err_t res = ESP_ERR_INVALID_ARG;
    for (u_int8_t i = 0; i < BUTTON_ADC_MAX; i++)
        if (buttons[i] == btn)
        {
            buttons[i] = NULL;
            res = ESP_OK;
            break;
        }
    CHECK(esp_timer_start_periodic(timer, POLL_TIMEOUT_US));
    return res;
};

//////////////// TESTING ZONE /////////////////////
static void button_adc_test(void *arg)
{
    int adc_reading;
    adc_oneshot_read(adc_handle, ADC_CHANNEL, &adc_reading);
    ESP_LOGI(TAG, "ADC Value = %i", adc_reading);
};

static const esp_timer_create_args_t timer_args_test = {
    .arg = NULL,                       //!< Argument to pass to the callback
    .name = "buttons_testing",         //!< Timer name, used in esp_timer_dump function
    .dispatch_method = ESP_TIMER_TASK, //!< Call the callback from task or from ISR
    .callback = button_adc_test,       //!< Function to call when timer expires
};

esp_err_t button_adc_init_test()
{
    ESP_LOGI(TAG, "Testing Buttons ADC Values");
    CHECK(esp_timer_create(&timer_args_test, &timer));
    init_adc();
    CHECK(esp_timer_start_periodic(timer, POLL_TIMEOUT_US * 30));
    return ESP_OK;
}

/* problemas


3- Tareas:
        chequear las
            funciones button_get_pressed_id
            button_is_pressed
            button_adc_deinit y luego cargar otro
            button_adc_testing()
            poll_buttons(void *arg)






*/