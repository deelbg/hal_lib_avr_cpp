// Example uses the following inputs:
// For Arduino UNO with atmega 328.
// Pin name         - Arduino pin name
// INPUT_RAW        - D8 
// INPUT_DEBAUNCED  - D9 
// INPUT_ENCODER_A  - D10 
// INPUT_ENCODER_B  - D11 
// OUTPUT_LED_1     - D13 
// OUTPUT_LED_2     - D12 

// Uncomment the examples to be executed.
#define HAL_IO_EXAMPLE    
// #define HAL_EEPROM_EXAMPLE  
// #define HAL_I2C_EXAMPLE
// #define HAL_AT24CXX_EEPROM_EXAMPLE

#ifdef HAL_IO_EXAMPLE
#include "timer.h"
#include "hal_avr_io.h"
#include "hal_encoder.h"
#endif // HAL_IO_EXAMPLE

#ifdef HAL_EEPROM_EXAMPLE
#include "hal_avr_eeprom.h"
#endif // HAL_EEPROM_EXAMPLE

#ifdef HAL_I2C_EXAMPLE
#include "hal_avr_i2c.h"
#endif

#ifdef HAL_AT24CXX_EEPROM_EXAMPLE
#include "hal_at24cxx_eeprom.h"
#endif



// Global variables
// =====================================================================
#if defined (HAL_I2C_EXAMPLE) || defined(HAL_AT24CXX_EEPROM_EXAMPLE)
static const uint8_t AT24C32_ADDRESS   = 0x57u;
c_i2c g_i2c         = c_i2c();
#endif // HAL_I2C_EXAMPLE

#ifdef HAL_IO_EXAMPLE
c_io g_io           = c_io();
c_timer g_timer_ms  = c_timer();
c_encoder g_encoder = c_encoder(g_io, false, g_io.INPUT_ENCODER_A, g_io.INPUT_ENCODER_B);
#endif // HAL_IO_EXAMPLE

#ifdef HAL_EEPROM_EXAMPLE
c_eeprom eeprom     = c_eeprom();
#endif // HAL_EEPROM_EXAMPLE

#ifdef HAL_AT24CXX_EEPROM_EXAMPLE
c_at24cxx_eeprom g_at24cxx_eeprom = c_at24cxx_eeprom(g_i2c, AT24C32_ADDRESS, c_at24cxx_eeprom::AT24C32);
#endif


void setup()
{    
    char str_write[5] = {'T', 'E', 'S', 'T', '\0'};
    char str_read[5] = {};
    uint8_t byte_read = 0u;
    uint16_t test_address = 0u;

    Serial.begin(115200);

#if defined (HAL_I2C_EXAMPLE) || defined(HAL_AT24CXX_EEPROM_EXAMPLE)
    Serial.println("\nInit I2C at 400 kHz");
#endif

#ifdef HAL_IO_EXAMPLE
    timer_0_init();    
    
    g_io.init();  
    g_io.pin_config_input_pullup(g_io.INPUT_RAW);
    g_io.pin_config_input_pullup(g_io.INPUT_DEBAUNCED);
    g_io.pin_config_input_pullup(g_io.INPUT_ENCODER_A);
    g_io.pin_config_input_pullup(g_io.INPUT_ENCODER_B);
    g_io.pin_config_output(g_io.OUTPUT_LED_1);
#endif // HAL_IO_EXAMPLE

#ifdef HAL_I2C_EXAMPLE  
    // at24c32 EEPROM - using directly i2c, write "0123456789" to EEPROM starting at test_address.
    // Read them back and print on serial.  
    // After each operation check the status ot the i2c interface.
    test_address = 10u;
    Serial.println("\nI2c write to eeprom");

    // Use the return value to validate operation result.
    if (!g_i2c.start_write(AT24C32_ADDRESS))
    {
        Serial.println("ERROR start or slave address");
    }
    else
    {
        // Use status(); function to validate operation result.
        g_i2c.write_byte((test_address & 0x0F00) >> 8u);

        if (c_i2c::I2C_WRITE_DATA_ACK != g_i2c.status())
        {
            Serial.println("ERROR eeprom address");
        }
        else
        {
            if (!g_i2c.write_byte((test_address & 0x00FF)))
            {
                Serial.println("ERROR eeprom address");
            }
            else
            {

                for (uint8_t index = 0; index < 10; index++)
                {    
                    Serial.print(index);
                    
                    if (!g_i2c.write_byte(index))
                    {
                        Serial.println("ERROR data to write");
                        
                        break;
                    }
                }
            }
        }

        g_i2c.stop();
        delay(10);

        Serial.println("\nRead");

        if(!g_i2c.start_write(AT24C32_ADDRESS))
        {
            Serial.println("ERROR start or slave address");
        }
        else
        {
            if (!g_i2c.write_byte((test_address & 0x0F00) >> 8u))
            {
                Serial.println("ERROR eeprom address MSB");
            }
            else
            {
                if (!g_i2c.write_byte(test_address & 0x00FF))
                {
                    Serial.println("ERROR eeprom address LSB");
                }

                if (!g_i2c.start_read(AT24C32_ADDRESS))
                {
                    Serial.println("ERROR start read");
                }
                else
                {   
                    for (uint8_t index = 0; index <= 8u; index++)
                    {
                        Serial.print(g_i2c.read_byte(c_i2c::ACK));

                        // Validate result of read operation through status(); function.
                        if (c_i2c::I2C_READ_DATA_ACK != g_i2c.status())
                        {
                            Serial.println("ERROR data read");
                        }
                    }

                    Serial.println(g_i2c.read_byte(c_i2c::NACK));
                    // Validate result of read operation through status(); function.
                    if (c_i2c::I2C_READ_DATA_NACK != g_i2c.status())
                    {
                        Serial.println("ERROR data read");
                    }
                }
            }
        }

        g_i2c.stop();
    }

    Serial.println("Done");
#endif // HAL_I2C_EXAMPLE

#ifdef HAL_AT24CXX_EEPROM_EXAMPLE    
    test_address = 130u;
    // at24c0x EEPROM - write "TEST1" to EEPROM starting at test_address.
    // Read them back and print on serial.
    Serial.println("\nWrite to hal_at24cxx_eeprom eeprom through i2c");

    // write a block of bytes to on chip EEPROM
    g_at24cxx_eeprom.write_block(str_write, test_address, sizeof(str_write));
    // write a single byte to on chip EEPROM
    g_at24cxx_eeprom.write_byte(test_address + 4u, 1u);    delay(10);
    Serial.print(str_write);
    Serial.println('1');

    Serial.println("Read");
    memset(str_read, 0u, sizeof(str_read));
    // read a block of bytes to on chip EEPROM
    g_at24cxx_eeprom.read_block(str_read, test_address, sizeof(str_read) - 1u);
    Serial.print(str_read);
    // read a single byte to on chip EEPROM
    Serial.println(g_at24cxx_eeprom.read_byte(test_address + 4u));
    Serial.println("Done");
#endif // HAL_AT24CXX_EEPROM_EXAMPLE

#ifdef HAL_EEPROM_EXAMPLE
    test_address = 30u;
    // EEPROM - write "TEST2" to EEPROM starting at test_address.
    // Read them back and print on serial.
    Serial.println("\nWrite to on chip eeprom");

    // write a block of bytes to on chip EEPROM
    eeprom.write_block(str_write, test_address, sizeof(str_write));
    // write a single byte to on chip EEPROM
    eeprom.write_byte(test_address + 4u, 2u);    
    Serial.print(str_write);
    Serial.println('2');

    Serial.println("Read");
    memset(str_read, 0u, sizeof(str_read));
    // read a block of bytes to on chip EEPROM
    eeprom.read_block(str_read, test_address, sizeof(str_read) - 1u);
    Serial.print(str_read);
    // read a single byte to on chip EEPROM
    Serial.println(eeprom.read_byte(test_address + 4u));
    Serial.println("Done");
#endif // HAL_EEPROM_EXAMPLE
}


void loop()
{
#ifdef HAL_IO_EXAMPLE
    bool input_raw = false;
    bool input_debaunced = false;
    static bool input_raw_old = false;
    static uint16_t input_raw_counter = 0u;
    static bool input_debaunced_old = false;    
    static uint16_t input_debaunced_counter = 0u;
    static bool flip_flop = false;

    // Raw input read.
    input_raw = g_io.input_get_raw(g_io.INPUT_RAW);

    if (input_raw != input_raw_old)
    {
        input_raw_old = input_raw;
        input_raw_counter++;
    }    

    // Debaunced input read.
    input_debaunced = g_io.input_get(g_io.INPUT_DEBAUNCED);

    if (input_debaunced != input_debaunced_old)
    {
        input_debaunced_old = input_debaunced;
        input_debaunced_counter++;
    }    

    // Each second flip the state of an output.
    // Display the pulses from rotary encoder, 
    // left and right for the last 1 second.
    // Display how many times raw and debaunced inputs had changed.
    if (g_timer_ms.expired())
    {        
        // Restart the timer.
        g_timer_ms.start(2000u);

        Serial.println("\n--------------------------"); 

        // Display the number of raw input state changes.
        Serial.print("Input raw: "); 
        Serial.println(input_raw_counter);
        input_raw_counter = 0u;

        // Display the number of debaunced input state changes.
        Serial.print("Input debaunced: "); 
        Serial.println(input_debaunced_counter);
        input_debaunced_counter = 0u;

        // Display the pulses from rotary encoder.
        Serial.print("Encoder left: "); 
        Serial.println(g_encoder.get_pulses_left(true));

        Serial.print("Encoder right: "); 
        Serial.println(g_encoder.get_pulses_right(true));


        // Flip the state of an output
        flip_flop = !flip_flop;

        if (flip_flop)
        {
            g_io.output_set(g_io.OUTPUT_LED_1);
        }
        else
        {
            g_io.output_reset(g_io.OUTPUT_LED_1);
        }       
    }    
#endif // HAL_IO_EXAMPLE
}

#ifdef HAL_IO_EXAMPLE
void timer_0_init(void)
{
    cli();//stop interrupts
    //set timer0 interrupt at 10kHz
    TCCR0A = 0; // set entire TCCR0A register to 0
    TCCR0B = 0; // same for TCCR0B
    TCNT0  = 0; // initialize counter value to 0
    // set compare match register for 2khz increments
    OCR0A = 24;// = (16*10^6) / (10000*64) - 1 (must be <256)
    // turn on CTC mode
    TCCR0A |= (1 << WGM01);
    // Set CS01 and CS00 bits for 64 prescaler
    TCCR0B |= (1 << CS01) | (1 << CS00);   
    // enable timer compare interrupt
    TIMSK0 |= (1 << OCIE0A);

    sei();//allow interrupts
}


//timer0 interrupt every 0.1 ms (10kHz)
ISR(TIMER0_COMPA_vect)
{
    static uint8_t  timer_0_ms = 0u;

    g_io.debaunce();

    if (0u == timer_0_ms)
    {
        g_encoder.scan();

        g_timer_ms.decrement();

        timer_0_ms = 10u;
    }

    timer_0_ms--;
}
#endif // HAL_IO_EXAMPLE