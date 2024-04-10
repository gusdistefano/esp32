#include <stdio.h>
#include "buzzer.h"
#include "driver/gpio.h"
#include <esp_log.h>

#define BUZZER_PIN CONFIG_BUZZER_PIN
#define UART_NUM CONFIG_UART_NUMBER
#define UART_BAUD_RATE CONFIG_UART_BAUD_RATE
#define UART_TX_PIN CONFIG_UART_TX_PIN
#define UART_RX_PIN CONFIG_UART_RX_PIN
#define UART_RTS_PIN CONFIG_UART_RTS_PIN
#define UART_CTS_PIN CONFIG_UART_CTS_PIN

#define BUF_SIZE 1024
#define TASK_MEMORY 1024 * 2

static const char *TAG = "BuzzerTest ";

esp_err_t init_buzzer(void)
{
    gpio_reset_pin(BUZZER_PIN);
    gpio_set_direction(BUZZER_PIN, GPIO_MODE_OUTPUT);
    gpio_set_level(BUZZER_PIN, 1);
    return ESP_OK;
};

esp_err_t buzzer_alarm(int beep_duration, int silence_duration, int repetition)
{
    // check bad arguments
    if (beep_duration < 0 || silence_duration < 0 || repetition < 0)
    {
        return ESP_ERR_INVALID_ARG;
    };

    for (int i = 0; i < repetition; i++)
    {
        gpio_set_level(BUZZER_PIN, 0);
        vTaskDelay(pdMS_TO_TICKS(beep_duration)); // beep duration

        gpio_set_level(BUZZER_PIN, 1);
        vTaskDelay(pdMS_TO_TICKS(silence_duration)); // silent duration
    }
    return ESP_OK;
}

/////////////////////////  UART SECTION   ////////////////////////////////////////

static void uart_task(void *pvParameters)
{
    uint8_t *data = (uint8_t *)malloc(BUF_SIZE);
    size_t index = 0;

    int parameters[3]; // Stores the integer parameters duration, silence, repetitions.

    while (1)
    {
        for (int i = 0; i < 3; ++i)
        {
            memset(data, 0, BUF_SIZE);
            index = 0;
            ESP_LOGI(TAG, "Enter the parameters in this sequence: Duration, Silence, Repetitions");

            while (1)
            {
                int len = uart_read_bytes(UART_NUM, data + index, 1, pdMS_TO_TICKS(100));

                if (len == 0)
                {
                    continue;
                }

                uart_write_bytes(UART_NUM, (const char *)(data + index), len);

                // Check for Enter key (ASCII code 13)
                if (data[index] == 13)
                {
                    data[index] = '\0'; // Replace Enter with the null terminator
                    break;              //  Exit the loop when Enter is pressed.
                }

                index++;
            }

            // Convert the received string to an integer.
            parameters[i] = atoi((const char *)data);
            ESP_LOGI(TAG, "Data received: %s", data);
        }

        // Call the alarm function with the integer parameters
        ESP_LOGI(TAG, "Parameters: duration=%d, silence=%d, repetitions=%d",
                 parameters[0], parameters[1], parameters[2]);
        vTaskDelay(pdMS_TO_TICKS(300));
        buzzer_alarm(parameters[0], parameters[1], parameters[2]);
    }
}

static void init_uart(void)
{

    uart_config_t uart_config = {
        .baud_rate = UART_BAUD_RATE,           /*!< UART baud rate*/
        .data_bits = UART_DATA_8_BITS,         /*!< UART byte size*/
        .parity = UART_PARITY_DISABLE,         /*!< UART parity mode*/
        .stop_bits = UART_STOP_BITS_1,         /*!< UART stop bits*/
        .flow_ctrl = UART_HW_FLOWCTRL_DISABLE, /*!< UART HW flow control mode (cts/rts)*/
        .rx_flow_ctrl_thresh = 122,            /*!< UART HW RTS threshold*/
        .source_clk = UART_SCLK_APB,           /*!< UART source clock selection */

    };

    ESP_ERROR_CHECK(uart_param_config(UART_NUM, &uart_config));

    // No need to set the pins as the defaults are correct for UART0
    ESP_ERROR_CHECK(uart_set_pin(UART_NUM, UART_TX_PIN, UART_RX_PIN, UART_RTS_PIN, UART_CTS_PIN));

    uart_driver_install(UART_NUM, BUF_SIZE, 0, 0, NULL, 0);

    xTaskCreate(uart_task, "uart_task", TASK_MEMORY, NULL, 5, NULL);

    ESP_LOGI(TAG, "Init UART completed");
}

esp_err_t test_buzzer_alarm(void)
{
    init_uart();
    return ESP_OK;
}