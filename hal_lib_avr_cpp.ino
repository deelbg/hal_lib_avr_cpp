#define HAL_IO_EXAMPLE
#define HAL_ENCODER_EXAMPLE
#define HAL_EEPROM_EXAMPLE
#define HAL_I2C_EXAMPLE
#define HAL_AT24C0X_EEPROM_EXAMPLE

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

#ifdef HAL_AT24C0X_EEPROM_EXAMPLE
#include "hal_at24cxx_eeprom.h"
#endif



// Global variables
// =====================================================================
static const uint32_t CPU_FREQUENCY = 16000000u;

#if defined (HAL_I2C_EXAMPLE) || defined(HAL_AT24C0X_EEPROM_EXAMPLE)
static const uint32_t I2C_FREQUENCY = 400000u;
static const uint8_t AT24C02_ADDRESS   = 0x68u;
c_i2c g_i2c         = c_i2c();
#endif // HAL_I2C_EXAMPLE

#ifdef HAL_IO_EXAMPLE
c_io g_io           = c_io();
c_timer g_timer_ms  = c_timer();
c_encoder g_encoder = c_encoder(g_io, g_io.INPUT_ENCODER_A, g_io.INPUT_ENCODER_B);
#endif // HAL_IO_EXAMPLE

#ifdef HAL_EEPROM_EXAMPLE
c_eeprom eeprom     = c_eeprom();
#endif // HAL_EEPROM_EXAMPLE

#ifdef HAL_AT24C0X_EEPROM_EXAMPLE
c_at24c0x_eeprom g_at24c0x_eeprom = c_at24c0x_eeprom(g_i2c, AT24C02_ADDRESS, c_at24c0x_eeprom::AT24C02);
#endif


void setup()
{    
    char str_write[5] = {'T', 'E', 'S', 'T', '\0'};
    char str_read[5] = {};
    uint8_t test_address = 0u;

    Serial.begin(115200);

#if defined (HAL_I2C_EXAMPLE) || defined(HAL_AT24C0X_EEPROM_EXAMPLE)
    Serial.println("\nInit I2C at 400 kHz");
    g_i2c.init(CPU_FREQUENCY, I2C_FREQUENCY);
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
    // at24c0x EEPROM - using directly i2c, write "0123456789" to EEPROM starting at test_address.
    // Read them back and print on serial.  
    // After each operation check the status ot the i2c interface.
    test_address = 0u;
    Serial.println("\nI2c write to eeprom");
    
    g_i2c.start_write(AT24C02_ADDRESS);

    if (g_i2c.WRITE_ADDRESS_OK != g_i2c.status())
    {
        Serial.println("ERROR start or slave address");
    }

    g_i2c.write_byte(test_address);

    if (g_i2c.WRITE_DATA_OK != g_i2c.status())
    {
        Serial.println("ERROR eeprom address");
    }

    for (uint8_t index = 0; index < 10; index++)
    {    
        Serial.print(index);
        g_i2c.write_byte(index);

        if (g_i2c.WRITE_DATA_OK != g_i2c.status())
        {
            Serial.println("ERROR data to write");
        }
    }

    g_i2c.stop();

    Serial.println("\nRead");
    
    for (uint8_t index = 0; index < 10; index++)
    {
        uint8_t byte_read = 0u;

        g_i2c.start_write(AT24C02_ADDRESS);

        if(g_i2c.WRITE_ADDRESS_OK != g_i2c.status())
        {
            Serial.println("ERROR start or slave address");
        }

        g_i2c.write_byte(test_address + index);

        if (g_i2c.WRITE_DATA_OK != g_i2c.status())
        {
            Serial.println("ERROR eeprom address");
        }

        g_i2c.start_read(AT24C02_ADDRESS);

        if (g_i2c.READ_ADDRESS_OK != g_i2c.status())
        {
            Serial.println("ERROR start or slave address");
        }

        byte_read = g_i2c.read_byte(g_i2c.NO_ACK);

        if (g_i2c.READ_DATA_OK != g_i2c.status())
        {
            Serial.println("ERROR data read");
        }
        else
        {
            Serial.print(byte_read);
        }

        g_i2c.stop();
    }

    Serial.println("\nDone");
#endif // HAL_I2C_EXAMPLE

#ifdef HAL_AT24C0X_EEPROM_EXAMPLE    
    test_address = 10u;
    // at24c0x EEPROM - write "TEST1" to EEPROM starting at test_address.
    // Read them back and print on serial.
    Serial.println("\nWrite to hal_at24c0x_eeprom eeprom through i2c");

    // write a block of bytes to on chip EEPROM
    g_at24c0x_eeprom.write_block(str_write, test_address, sizeof(str_write));
    // write a single byte to on chip EEPROM
    g_at24c0x_eeprom.write_byte(test_address + 4u, 1u);    
    Serial.print(str_write);
    Serial.println('1');

    Serial.println("Read");
    memset(str_read, 0u, sizeof(str_read));
    // read a block of bytes to on chip EEPROM
    g_at24c0x_eeprom.read_block(str_read, test_address, sizeof(str_read) - 1u);
    Serial.print(str_read);
    // read a single byte to on chip EEPROM
    Serial.println(g_at24c0x_eeprom.read_byte(test_address + 4u));
    Serial.println("Done");
#endif // HAL_AT24C0X_EEPROM_EXAMPLE

#ifdef HAL_EEPROM_EXAMPLE
    test_address = 20u;
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

    // Raw input read, print the state on change.
    input_raw = g_io.input_get_raw(g_io.INPUT_RAW);

    if (input_raw != input_raw_old)
    {
        input_raw_old = input_raw;
        input_raw_counter++;
    }    

    // Debaunced input read, print the state on change.
    input_debaunced = g_io.input_get(g_io.INPUT_DEBAUNCED);

    if (input_debaunced != input_debaunced_old)
    {
        input_debaunced_old = input_debaunced;
        input_debaunced_counter++;
    }    

    // Each second flip the state of an output.
    // Display the pulses from rotary encoder, 
    // left and right for the last 1 second.
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