## ESP-IDF Extra Components library

## How to use

### ESP32-xx

Clone repository, e.g.:

```Shell
cd /path_to_components_directory
git clone https://github.com/gusdistefano/esp32.git
```

Add path to components in your [CMakeLists.txt](https://docs.espressif.com/projects/esp-idf/en/latest/esp32/api-guides/build-system.html):
e.g:

```CMake
cmake_minimum_required(VERSION 3.5)
set(EXTRA_COMPONENT_DIRS /path_to_components_directory/esp32/components)
include($ENV{IDF_PATH}/tools/cmake/project.cmake)
project(esp32-project)
```

## Components

### ADC/DAC libraries

| Component           | Description                                      | License | Supported on | Thread safety |
| ------------------- | ------------------------------------------------ | ------- | ------------ | ------------- |
| **button_adc**      | Driver button ADC                                | MIT     | esp32,       |               |
| **button_adc_lvgl** | Driver for button ADC designed for use with LVGL | MIT     | esp32        |               |

### Other misc libraries

| Component  | Description       | License | Supported on | Thread safety |
| ---------- | ----------------- | ------- | ------------ | ------------- |
| **buzzer** | Driver for Buzzer | MIT     | esp32        |               |

### Examples

To use the examples, simply open the directory of the component to be tested with Visual Studio Code or Eclipse and compile.
Remember to run `idf.py menuconfig` to configure the component
