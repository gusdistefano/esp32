/*
 * The MIT License (MIT)
 *
 * Copyright (c) 2024 Gus Di Stefano <narniancoder@gmail.com>
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 */

/**
 * @file button_adc_lvgl.h
 * @defgroup button_adc_lvgl
 * @{
 *
 * ESP-IDF driver button ADC for use with LVGL
 *
 * Copyright (c) 2024 Gus Di Stefano <narniancoder@gmail.com>
 *
 * MIT Licensed as described in the file LICENSE
 */

#ifndef __COMPONENTS_BUTTON_ADC_LVGL_H__
#define __COMPONENTS_BUTTON_ADC_LVGL_H__

#include <stdint.h>
#include <stdbool.h>
#include <esp_err.h>
#include "esp_adc/adc_oneshot.h"

#ifdef __cplusplus
extern "C"
{
#endif

    typedef enum
    {
        BUTTON_RELEASED = 0,
        BUTTON_PRESSED,
    } button_state_t;

    typedef struct
    {
        uint8_t button_index; /**< button index on the channel */
        uint16_t lower_limit; //!< ADC lower input range
        uint16_t upper_limit; //!< ADC upper input range
        button_state_t state; //!< State RELEASE / PRESSED
    } button_adc_config_t;

    /**
     * @brief Init button,
     * @param btn Pointer to button descriptor
     * @return `ESP_OK` on success
     */

    esp_err_t button_adc_init(button_adc_config_t *btn);

    /**
     * @brief Deinit button
     *
     * @param btn Pointer to button descriptor
     * @return `ESP_OK` on success
     */
    esp_err_t button_adc_deinit(button_adc_config_t *btn);

    /**
     * @brief Get the ID (0, 1, 2, ...) of the pressed button. Returns -1 if none
     * @return -1 if none
     */

    /*Get ID  (0, 1, 2 ..) of the pressed button devuelve -1 si no hay ninguno*/
    int8_t button_adc_get_pressed_id(void);

    /**
     * @brief Test if button is pressed or not
     *
     * @param btn Pointer to button descriptor
     * @return true or false
     */
    bool button_adc_is_pressed(button_adc_config_t *btn);

    /**
     * @brief Display the ADC readings, so that the buttons can be configured later
     * @param
     * @return ESP_OK
     */
    esp_err_t button_adc_init_test();
    

#ifdef __cplusplus
}
#endif

/**@}*/

#endif /* __COMPONENTS_BUTTON_ADC_LVGL_H__ */