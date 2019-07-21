# hal_lib_avr_cpp
This collection of driver classes provides a Hardware Abstraction Layer (HAL) library for use with AVR (ATTINY and ATMEGA) microcontrollers and is written in C++.

The drivers are intended to be used in projects based on AVR microcontrollers that are not developped using the arduino IDE and libraries. Despite that they might be used with Arduino hardware and can be compiled in Arduino IDE since it uses GCC compiler as well.

### The following drivers are provided:  
*hal_avr_io.h* - Digital IO driver.  
*hal_encoder.h* - Rotary encoder driver.  
*hal_avr_i2c.h* - i2c driver for microcontrollers with TWI hardware (ATMEGA) or USI hardware (ATTINY).  
*hal_avr_eeprom.h* - Driver for the internal EEPROM of AVR microcontrollers.  
*hal_at24cxx_eeprom.h* - Driver for EEPROM chips AT24C01, AT24C02, AT24C04, AT24C08, AT24C16, AT24C32, AT24C64.  
Complete description of all classes and their public interface is provided in [**description.md**](./description.md). 

### Examples:  
Examples of usage of all drivers are given in an Arduino sketch [**hal_lib_avr_cpp.ino**](./hal_lib_avr_cpp.ino) for easier demonstration. Example explanations are in the comments. 

### License:  
MIT License for open sourse software. The complete text of the license might be found in the [**LICENSE**](./LICENSE) file. 

