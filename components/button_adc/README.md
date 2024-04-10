# Button ADC Driver

This code implements a C library for handling buttons in embedded systems using ADC readings. These buttons are configured based on the circuit shown below

![scheme.png](/Datos/Final/drivers-gus/examples/button_adc/images/scheme.png)



This circuit allows multiple readings from voltage dividers, each registered as a specific button. It's important to note that there is a 'range of values' associated with each button, depending on the specific circuit.

### File .h (button_adc.h)

#### Data Types and Enumerations

1. `button_t`: Structure that describes a button, with details such as the button number, ADC limits, activation of autorepeat, and a callback.

2. `button_state_t`: Enumeration representing the states or events of the button (Pressed, Released, Clicked, Long Pressed).

3. `button_event_cb_t`: Callback function prototype called when a button event occurs. (`callback`)

#### Functions

4. `button_init`: Initializes a button. If the `btn` parameter is null, it acts as a test to visualize the ADC value range through the terminal.

5. `button_init_test`: Initializes TEST MODE

6. `button_done`: Deinitializes a button.

### 

### File .c (button_adc.c)

#### Variables and Macros

6. `DEAD_TIME_US`: Dead time to ignore inputs after a button is pressed.

7. `ADC_CHANNEL`: Configured ADC channel for reading values.

8. `POLL_TIMEOUT_US`, `POLL_TIMEOUT_TEST_US`, `AUTOREPEAT_TIMEOUT_US`, `AUTOREPEAT_INTERVAL_US`, `LONG_PRESS_TIMEOUT_US`: Time parameters to control button behavior.

9. `buttons`: Array of pointers to `button_t` structures representing configured buttons.

10. `timer`: Timer handler used for periodic ADC reading and button event management.

#### Functions

11. `init_adc`: Initializes and configures the ADC.

12. `poll_button`: Main function to handle the button's state and events.

13. `poll`: Periodic callback function to process buttons.

14. `button_test`: Test function that prints ADC values through the terminal.

15. `button_init`: Initializes buttons and sets up the timer for periodic reading.

16. `button_done`: Deinitializes buttons and stops the timer.

### Key Features

- Handling of multiple buttons with individual configurations.
- Detection of events such as presses, clicks, and long presses.
- Configuration of ADC limits for each button.
- Ability to perform button tests by visualizing ADC values through the terminal.

This summary provides an overview of the functions and features of the code.

### Usage

Add the `buzzer driver` directory within the `components` directory.
Then, in the 'main' directory, in the CMakeLists.txt file,

register the component:

```cmake
idf_component_register(SRCS "main.c"
 INCLUDE_DIRS ".")
list(APPEND EXTRA_COMPONENT_DIRS components)
```

Afterward, include it in main.c:

```c
#include "button_adc.h"
```

**NOTE**: Its necessary to configure parameters via `idf.py menuconfig`

 ------>      `********Only works for ADC1********`     <---------

```c
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

static button_t btn1;

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

    ESP_ERROR_CHECK(button_init(&btn1)); //Inicialice btn1
    ESP_ERROR_CHECK(button_init(&btn2)); //Inicialice btn2

   // ESP_ERROR_CHECK(button_init_test());  //TEST MODE
   // code ...
}
```
