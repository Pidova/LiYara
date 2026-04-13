# Documentation
LiYara is a lightweight fast LED display library for displaying images through LED matrixes or LED strips.

## Table of Contents
- [Overview](#Overview)
- [Configs](#Configs)
- [Support](#Support)
- [ISA Design](#ISA)

## Overview
Arduino is the primary supported platform for LiYara. 
While additional microcontrollers may be supported in the future adding your own Platform is very straight forward as only pin data and sdcard handling are currently the only platform specific code. Liyara works using bytecode interpreted by a virtual machine meaning you can pass code onto it during runtime and execute it. The current model is **DATA -> Interpreter -> Execution**.

## Configs
There are several configuartions offered either through macros or settings
### Settings
- [Interpreter Configs](../shared/interpreter_config.hpp)
- [Display Configs](../shared/display_config.hpp)
### Macros
- **LIYARA_EXTRAS_SD_ENABLE** Enables SD card wrapper


## Support
Current LED modules supported below:
- ADAFRUIT-WS2812


## ISA
LiYara executes programs defined by its custom ISA.
See full specification: [ISA Documentation](ISA.md)