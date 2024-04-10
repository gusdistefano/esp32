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
 * @file buzzer.h
 * @defgroup buzzer
 * @{
 *
 * ESP-IDF driver Buzzer
 *
 * Copyright (c) 2024 Gus Di Stefano <narniancoder@gmail.com>
 *
 * MIT Licensed as described in the file LICENSE
 */

#ifndef __COMPONENTS_BUZZER_H__
#define __COMPONENTS_BUZZER_H__

#include <string.h>
#include "driver/uart.h"


#include <esp_err.h>

#ifdef __cplusplus
extern "C"
{
#endif

    /**
     * @brief Init Buzzer. Initializes the buzzer. Use menuconfig to configure the pins.
     * @param void
     * @return `ESP_OK` on success
     */
    esp_err_t init_buzzer(void);

    /**
     * @brief Buzzer Alarm. Activates the buzzer with three parameters, allowing different sounds.
     * @param beep_duration Duration of the beep in milliseconds.
     * @param silence_duration Duration of the silence between beeps in milliseconds.
     * @param repetition Number of times the beep pattern should be repeated.
     * @return `ESP_OK` on success
     */
    esp_err_t buzzer_alarm(int beep_duration, int silence_duration, int repetition);

    /**
     * @brief Test Buzzer Alarm.
     * Allows console input for reading different values of beep duration, silence duration, and repetition.
     * Listens to the emitted sound based on the provided parameters.
     * The entered parameter values are used to call the `buzzer_alarm` function.
     *
     * @details
     * The test scenario involves reading three values from the console:
     * - Beep Duration: Duration of the beep sound in milliseconds.
     * - Silence Duration: Duration of the silence between beeps in milliseconds.
     * - Repetition: Number of times the beep pattern should be repeated.
     *
     * The entered values are used as parameters to call the `buzzer_alarm` function, allowing you to
     * hear the sound pattern based on the provided parameters.
     *
     * @note Ensure the `buzzer_alarm` function is properly initialized before running this test.
     */
    esp_err_t test_buzzer_alarm(void);

#ifdef __cplusplus
}
#endif

/**@}*/

#endif /* __COMPONENTS_BUZZER_H__ */
