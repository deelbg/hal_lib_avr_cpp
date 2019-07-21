# hal_lib_cpp
Hardware abstraction layer library for use with AVR (ATTINY and ATMEGA) microcontrollers written in C++.

<!-- ======================================================================= -->
<!-- ======================================================================= -->

## *hal_avr_io.h* - Digital IO driver: 
An IO driver class. Provides functions to easily configure pins as digital inputs, with or without internal pullps, outputs. 
Functions to read inputs and set or reset outputs are provided as well. There is a software debaunce functionality for the digital inputs.
In order to make the class usable, some settings need to be done in *`hal_avr_io.h`* file.  

**Step 1:** Comment or uncomment the needed defines for the ports which a particular microcontroller has. This is located in the beggining of the file.  
*`#define HAS_PORT_A`*  
*`#define HAS_PORT_B`*  
*`#define HAS_PORT_C`*  
*`#define HAS_PORT_D`*  
*`#define HAS_PORT_E`*  
*`#define HAS_PORT_F`*  
Comment out the unneeded defines. For example ATTINY 841 has only PORTA and PORTB available. 

**Step 2:** List all the digital inputs and outputs which are needed in the *`enum io_pins_t{}`*. If there are more than 32 pins used, the inputs should be listed before the outputs. The reason is that the software debaunce works with only 32 pins. Use names that are best for your application like *`BUTTON_OK`*, *`ENCODER_A`*, *`ENCODER_B`* etc. and keep the *`NUMBER_OF_IO_PINS`* entry at the end.

**Step 3:** Find the constant array *`const pin_t IO_PIN[NUMBER_OF_IO_PINS]`* and enter the port and pin number for each io line described in the enum mentioned in Step 2.
With these 3 steps the setup is done and digital inputs and outputs can be changed and read by the io driver.


#### *c_io();*  
The constructor takes no parameters.

#### *void init(void);*
Initialize all of the microcontroller's IO pins to inputs and disables internall pullups. 

#### *void pin_config_input(io_pins_t pin_number);*  
Configures the pin *`pin_number`* as a digital input with pullup disabled.

#### *void pin_config_input_pullup(io_pins_t pin_number);*  
Configures the pin *`pin_number`* as a digital input with pullup enabled.

#### *void pin_config_output(io_pins_t pin_number);*  
Configures the pin *`pin_number`* as a digital output.

#### *bool input_get_raw(io_pins_t pin_number);*  
Returns the state of pin *`pin_number`* if it is set as an input. In case the pin was set as an output the function allways returns *`false`*; 
The momentary state of the pin is reported. The software debaunce is not applied here. 

#### *bool input_get(io_pins_t pin_number);*  
Returns the state of pin *`pin_number`* if it is set as an input. In case the pin was set as an output the function allways returns *`false`*; 
The software debaunce is applied here. In order to return true, the input pin must have been in a steady state for at least *`INPUT_DEBAUNCE_SAMPLES`* calls of the *`debaunce();`* function.  

#### *void output_set(io_pins_t pin_number);*  
Sets the pin *`pin_number`* high. No action is taken if the pin is configured as an input.

#### *void output_reset(io_pins_t pin_number);*  
Sets the pin *`pin_number`* low. No action is taken if the pin is configured as an input.

#### *void debaunce(void);*  
Must be called in a timer interrupt. 
This function is crutial for the software debaunce of the digital inputs. I collects the history of the first 32 pins in the *`io_pins_t`* enum for *`INPUT_DEBAUNCE_SAMPLES`* calls. The result is used in *`input_get();`*. 

<!-- ======================================================================= -->
<!-- ======================================================================= -->

## *hal_encoder.h* - Rotary encoder driver: 
Provides an encoder class, named *`c_encoder`* which reads a rotary encoder and provides a way to determine the number of pulses the encoder had made in both directions. The class is intended to be used with a hand turned rotary encoders for the purpusses of a human interface implementation. An object of *`c_io`* class is used to access the input pins. It is possible to select whether the software debaunce provided by *`c_io`* class is to be used or raw io data is going to be used and the input debaunce is managed in hardware. For application where low clock frequency is used, the raw input data and hardware debaunce is the way to go.  

#### *c_encoder(c_io & ref_io, bool use_raw_input, uint8_t encoder_pin_a, uint8_t encoder_pin_b);*
The constructor takes the following parameters:  
*`ref_io`* - an object of class *`c_io`*, given by reference.
*`use_raw_input`* - *`bool`*, defines where the *`scann();`* function uses the software debaunced io pins or raw undebainced io pin data.  
*`encoder_pin_a`* - The number of encoder's pin A from *`io_pins_t`* enum in *hal_avr_io.h*.  
*`encoder_pin_b`* - The number of encoder's pin B from *`io_pins_t`* enum in *hal_avr_io.h*.  

#### *void scan(void);*
Reads the input pins to which rotarry encoder A and B signals are connected. Should be called in a timer interrupt.
If the constructor was used with *`use_raw_input = false;`* The software debaunce for the inputs is used and the *`scan();`* function should be called at at least 5 times, preferably 10 times, greater period than the *`debaunce();`* function of the IO object. For example if *`debaunce();`* is called every 0.1mS, the *`scan();`* might be called every 1mS and this gives a good response for a hand turned encoder wheel.
If raw input data is used, the software debaunce in the IO object is not used, so a hardware debaunce of the pins is needed. 

#### *uint8_t get_pulses_left(bool reset_pulses);*
Returns the number of pulses in the left direction made since the last call of the function with *`reset_pulses = true;`*
Parameter *`reset_pulses`* defines if the internal pulse counter should be cleared.

#### *uint8_t get_pulses_right(bool reset_pulses);*
Returns the number of pulses in the right direction made since the last call of the function with *`reset_pulses = true;`*
Parameter *`reset_pulses`* defines if the internal pulse counter should be cleared.

#### *void set_pulses_left(uint8_t pulses);*
Sets the internal counter for pulses in left direction to the value given in the *`pulses`* parameter.

#### *void set_pulses_right(uint8_t pulses);*
Sets the internal counter for pulses in right direction to the value given in the *`pulses`* parameter.

<!-- ======================================================================= -->
<!-- ======================================================================= -->

## *hal_avr_i2c.h* - i2c driver for microcontrollers with TWI hardware (ATMEGA) or USI hardware (ATTINY): 
Class *`c_i2c()`* that provides an i2c driver. It provides functions to initialize the interface, send and receive data through it.

There is a bit of configuration to be made:  
*`#define F_CPU 4000000UL`*  - find this definition in the *`hal_avr_i2c_usi.c`* file and adjuct it to the clock frequency of your application.
*`#define I2C_FAST_MODE_400KHZ`* - comment this line in out *`hal_avr_i2c_usi.h`* file if you want to run the i2c bus at 100kHz or leave it uncommented for 400kHz.
*`#define I2C_HARDWARE_USI`* - uncomment this line if your microcontroller has the USI hardware (ATTINY). Leave it commented if it has the TWI hardware (ATMEGA).

For devices witht the USI hardware (ATTINY), adjust the definitions following *`#define I2C_PINS`*.

#### *c_i2c();*
The constructor takes no parameters.

#### *bool start_write(uint8_t slave_address);*  
Starts a write session. *`slave_address`* parameter takes the address of the i2c device on the bus with which communication is initiated.

#### *bool start_read(uint8_t slave_address);*  
Starts a read session. *`slave_address`* parameter takes the address of the i2c device on the bus with which communication is initiated.

#### *void stop(void);*
End a read/write session.

#### *bool write_byte(uint8_t byte);*  
Write a byte to the i2c bus. Takes as a parameter the byte to be sent. Returns *`true`* if on success, otherways *`false`*.

#### *uint8_t read_byte(i2c_receive_response_t send_ack);*
Read a byte from the i2c deice. Returns the read byte. Take as a paramenter if ACK is to be sent or not.

#### *uint8_t status (void);*
For devices with TWI hardware will return the status TWI status. For devices with USI will return true.

In order to communicate to an i2c device, first start a session, then read/write bytes and after that close the session. 
```
Example:
c_i2c i2c = c_i2c();
uint8_t result = 0;

// Write 4 bytes to the i2c bus.
i2c.start_write(0x7A);
i2c.write_byte('t');
i2c.write_byte('e');
i2c.write_byte('s');
i2c.write_byte('t');
i2c.stop(); 

// Read a byte from address 0x01 in an i2c device with address 0x7A.
i2c.start_write(0x7A);
i2c.write_byte(0x01);
i2c.start_read(0x7A);
result = i2c.read_byte(c_i2c::NO_ACK);
i2c.stop();
```  

<!-- ======================================================================= -->
<!-- ======================================================================= -->

## *timer.h* - Software timer: 
A timer class that might be used to measure a precise time period. There is a *`tick();`* function that needs to be called in a timer interrupt to guarantee precise time measurement. The way to use it is add a call to the tick function in a timer ISR. Where time measurement is needed, call the *`start(time_to_measure);`* function and then wait for the *`expired();`* function to return true. The Exact time measured is time_to_measure multilied by the period of the timer interrupt.

#### *c_timer();*
The constructor takes no parameters.

#### *void start(uint16_t value);*
Starts the timer counting. It takes as a parameter the time period. If the timer is in mS, uS or seconds depends on the interrupt in which the *`tick();`* function is called.

#### *void tick(void);*
Decrements the timer's counter. To get a precise time measurement, this function should be called in a timer interrupt service routine. 

#### *bool expired(void);*
Returns *`true`* when the timer has expired, i.e. it's counter had counted down to zero. 
The actual time between calling *`start();`* and *`expired();`* returning true is the value used in calling *`start();`* multiplied by the time period between two calls of the *`tick();`* function, which would be the timer interrupt period.  

<!-- ======================================================================= -->
<!-- ======================================================================= -->

## *hal_avr_eeprom.h* - Driver for the internal EEPROM of AVR microcontrollers: 
Class c_eeprom. Provides a driver for the integrated EEPROM memmory in AVR microcontrollers. 

#### *c_eeprom();*  
The constructor takes no parameters.

#### *uint8_t read_byte(uint16_t address);*  
Reads a byte from address *`address`* of the EEPROM and returns it.

#### *void write_byte(uint16_t address, uint8_t byte);*  
Writes a byte *`byte`* to address *`address`* of the EEPROM.

#### *void read_block(uint8_t * const block, uint16_t address, uint16_t size);*  
Read a block of bytes form EEPROM adress *`address`*, *`size`* is the number of bytes to be read and *`block`* is s pointer to a buffer where read bytes are to be stored.

#### *void write_block(uint8_t const * const block, uint16_t address, uint16_t size);*  
Write a block of bytes to EEPROM adress *`address`*, *`size`* is the number of bytes to be written and *`block`* is s pointer to a buffer from which the bytes are to be copied into EEPROM.

<!-- ======================================================================= -->
<!-- ======================================================================= -->

## *hal_at24cxx_eeprom.h* - Driver for EEPROM chips of the at24 series: 
Class c_at24c0x_eeprom. Provides a driver for the AT24C0X EEPROM memmory chips connected via i2c to a AVR microcontrollers. 

#### *c_at24c0x_eeprom(c_i2c & ref_i2c, uint8_t i2c_address, chip_type_t type);*
Constructor, parameters are:  
*`ref_i2c`* - *`c_i2c`* object by reference. This it the interface through which the microcontroller is going to communicate to the EEPROM chip.
*`i2c_address`* - the i2c address of the EEPROM chip.

*`type`* - identifies the EEPROM chip type. The following are supported:  
*`AT24C01`*  
*`AT24C02`*  
*`AT24C04`*  
*`AT24C08`*  
*`AT24C16`*  
*`AT24C32`*  
*`AT24C64`*  

#### *uint8_t read_byte(uint16_t address);*
Reads a byte from address *`address`* of the EEPROM and returns it.

#### *void write_byte(uint16_t address, uint8_t byte);*
Writes a byte *`byte`* to address *`address`* of the EEPROM.

#### *void read_block(uint8_t * const block, uint16_t address, uint16_t size);* 
Read a block of bytes form EEPROM adress *`address`*, *`size`* is the number of bytes to be read and *`block`* is s pointer to a buffer where read bytes are to be stored.

#### *void write_block(uint8_t const * const block, uint16_t address, uint16_t size);* 
Write a block of bytes to EEPROM adress *`address`*, *`size`* is the number of bytes to be written and *`block`* is s pointer to a buffer from which the bytes are to be copied into EEPROM.

