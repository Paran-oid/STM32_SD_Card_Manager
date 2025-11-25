# Configurable Macros

The sca library provides several optional features that can be enabled by defining the corresponding macros in your main CMakeLists.txt file. These macros allow you to selectively include functionality such as SPI, UART, GPIO, timers, and the watchdog.

```
target_compile_definitions(sca PUBLIC
    SCA_GPIO
    SCA_IWDG
    SCA_SPI
    SCA_PTIMER
    SCA_UART)
```

Available Macros

SCA_GPIO — Enable GPIO-related functionality.

SCA_IWDG — Enable Independent Watchdog support.

SCA_SPI — Enable SPI communication support.

SCA_PTIMER — Enable periodic timer functionality.

SCA_UART — Enable UART communication support.


⚠️ Only define the macros for the features you actually need. Defining unnecessary macros may increase code size. ⚠️ 
