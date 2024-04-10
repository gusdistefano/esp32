## Buzzer

The driver allows emitting different alarms with the buzzer. It is connected to the selected GPIO pin via `idf.py menuconfig`

It also has a test_buzzer_alarm function to configure, via console, the three parameters of

- beep duration
- silence duration
- repetitions

With this function, it is possible to listen to the different alarms, and then use them in the call to the buzzer_alarm function.

## Usage

Add the 'buzzer driver' directory within the 'components' directory.
Then, in the 'main' directory, in the CMakeLists.txt file,

register the component:

```cmake
idf_component_register(SRCS "main.c"
 INCLUDE_DIRS ".")
list(APPEND EXTRA_COMPONENT_DIRS components)
```

Afterward, include it in main.c:

```c
#include "buzzer.h"
```

### Test Buzzer Functionality

This functionality allows console input for reading different values of beep duration, silence duration, and repetition. It listens to the emitted sound based on the provided parameters.

The entered parameter values are used to call the `buzzer_alarm` function.

The test scenario involves reading three values from the console:

- Beep Duration: Duration of the beep sound in milliseconds.
- Silence Duration: Duration of the silence between beeps in milliseconds.
- Repetition: Number of times the beep pattern should be repeated.

The entered values are used as parameters to call the `buzzer_alarm` function, allowing you to hear the sound pattern based on the provided parameters.

**Note**: Ensure the buzzer_alarm function is **properly initialized **before running this test.

```c
void main(){
    init_buzzer();
//    .... code
    test_buzzer_alarm();
}
```

It is necessary to configure the UART parameters via `idf.py menuconfig`

### The UART in ESP32

The ESP32 has more than one UART, depending on the model we are using. Specifically:

- ESP32 and ESP32-S3 have 3 UARTs
- ESP32-S3 and ESP32-C3 have 2 UARTs
  These ports are named UART0, UART1, and UART2. Each of them can use four pins: RX, TX, CTS, and RTS. However, the Arduino environment only uses RX and TX.

Another difference from ESP32 compared to a conventional Arduino is that, thanks to its multiplexer, we can reassign UARTs to any pin without performance loss.

UARTs come preconfigured to use certain pins. But we can, and sometimes must, change the pins.

These are the default assignments for ESP32:

| UART Port | TX  | RX  | RTS | CTS |
| --------- | --- | --- | --- | --- |
| UART0     | 1   | 3   | 22  | 19  |
| UART1     | 10  | 9   | 11  | 6   |
| UART2     | 17  | 16  | 7   | 8   |

As we can see, UART1's TX and RX pins coincide with those used to connect the SPI Flash memory. So, if we want to use UART1, we must necessarily reassign the pins.

These are the default assignments for ESP32-S3:

| UART Port | TX  | RX  | RTS | CTS |
| --------- | --- | --- | --- | --- |
| UART0     | 49  | 50  | 15  | 16  |
| UART1     | 17  | 18  | 19  | 20  |
| UART2     | -   | -   | -   | -   |

In this case, UART2 is not associated with any pin by default. So, if we want to use it, we'll need to reassign it to a pin of our choice."
