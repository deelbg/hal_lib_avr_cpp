#include <stdint.h>
#include "hal_avr_io.h"

c_io::c_io()
{

}

void c_io::init(void)
{
// Global pullups disable.
    MCUCR |= (1u << PUD);
// Set all as inputs, no pullups.
#ifdef HAS_PORT_A
    PORTA = PIN_NO_PULLUP;
    DDRA  = PIN_INPUT;
#endif

#ifdef HAS_PORT_B

    PORTB = PIN_NO_PULLUP;
    DDRB  = PIN_INPUT;
#endif

#ifdef HAS_PORT_C
    PORTC = PIN_NO_PULLUP;
    DDRC  = PIN_INPUT;
#endif

#ifdef HAS_PORT_D
    DDRD  = PIN_NO_PULLUP;
    PORTD = PIN_INPUT;
#endif

#ifdef HAS_PORT_E
    DDRE  = PIN_NO_PULLUP;
    PORTE = PIN_INPUT;
#endif

#ifdef HAS_PORT_F
    DDRF  = PIN_NO_PULLUP;
    PORTF = PIN_INPUT;
#endif 
}


void c_io::pin_config_input(uint8_t pin)
{
    if (NUMBER_OF_IO_PINS > pin)
    {
        // Set pin as input.
        ddrxn_clear(IO_PIN[pin].port, IO_PIN[pin].pin_number);
        // Pin speciffic pull up disable.        
        portxn_clear(IO_PIN[pin].port, IO_PIN[pin].pin_number);
    }
}


void c_io::pin_config_input_pullup(uint8_t pin)
{    
    if (NUMBER_OF_IO_PINS > pin)
    {
        // Global pullups enable.
        MCUCR &= ~(1u << PUD);
        // Set pin as input.
        ddrxn_clear(IO_PIN[pin].port, IO_PIN[pin].pin_number);
        // Pin speciffic pull up enable.
        portxn_set(IO_PIN[pin].port, IO_PIN[pin].pin_number);
    }
}


void c_io::pin_config_output(uint8_t pin)
{
    if (NUMBER_OF_IO_PINS > pin)
    {
        // Set as an output.
        ddrxn_set(IO_PIN[pin].port, IO_PIN[pin].pin_number);
        portxn_clear(IO_PIN[pin].port, IO_PIN[pin].pin_number);
    }
}


bool c_io::input_get_raw(uint8_t pin)
{
    bool result = false;

    if ((NUMBER_OF_IO_PINS > pin) &&
        (PIN_INPUT == ddrxn_get(IO_PIN[pin].port, IO_PIN[pin].pin_number)))
    {
        result = pinxn_get(IO_PIN[pin].port, IO_PIN[pin].pin_number);
    }

    return result;
}


bool c_io::input_get(uint8_t pin)
{
    bool result = false;
    uint32_t inputs_debaunced = 0xFFFFFFFFu;

    for (uint8_t index = 0u; index < INPUT_DEBAUNCE_SAMPLES; index++)
    {
        inputs_debaunced &= inputs_debaunce_buffer[index];
    }

    if ((0x01u << pin) & inputs_debaunced)
    {
         result = true;
    }

    return result;
}


void c_io::output_set(uint8_t pin)
{
    if ((NUMBER_OF_IO_PINS > pin) &&
        (PIN_OUTPUT == ddrxn_get(IO_PIN[pin].port, IO_PIN[pin].pin_number)))
    {
        portxn_set(IO_PIN[pin].port, IO_PIN[pin].pin_number);
    }
}


void c_io::output_reset(uint8_t pin)
{
    if ((NUMBER_OF_IO_PINS > pin) &&
        (PIN_OUTPUT == ddrxn_get(IO_PIN[pin].port, IO_PIN[pin].pin_number)))
    {
        portxn_clear(IO_PIN[pin].port, IO_PIN[pin].pin_number);
    }
}


void c_io::debaunce(void)
{
    static uint8_t input_debaunce_index = 0u;
    uint32_t tmp = 0u;

    for (uint8_t index = 0; index < NUMBER_OF_IO_PINS; index++)
    {
        if ((PIN_INPUT == ddrxn_get(IO_PIN[index].port, IO_PIN[index].pin_number)) &&
            (pinxn_get(IO_PIN[index].port, IO_PIN[index].pin_number)))
        {
            tmp |=  (0x01u << index);
        }
    }

    inputs_debaunce_buffer[input_debaunce_index] = tmp;
 
    input_debaunce_index++;
    
    if (input_debaunce_index >= INPUT_DEBAUNCE_SAMPLES )
    {
        input_debaunce_index = 0u;
    }
}


void c_io::portxn_set(uint8_t port, uint8_t pin)
{
    switch(port)
    {
#ifdef HAS_PORT_A
        case PORT_A:
            PORTA |= (1u << pin);
        break;
#endif

#ifdef HAS_PORT_B
        case PORT_B:
            PORTB |= (1u << pin);
        break;
#endif

#ifdef HAS_PORT_C
        case PORT_C:
            PORTC |= (1u << pin);
        break;
#endif

#ifdef HAS_PORT_D
        case PORT_D:
            PORTD |= (1u << pin);
        break;
#endif

#ifdef HAS_PORT_E
        case PORT_E:
            PORTE |= (1u << pin);
        break;
#endif

#ifdef HAS_PORT_F
        case PORT_F:
            PORTF |= (1u << pin);
        break;
#endif

        default:

        break;
    }
}


void c_io::portxn_clear(uint8_t port, uint8_t pin)
{
    switch(port)
    {
#ifdef HAS_PORT_A
        case PORT_A:
            PORTA &= ~(1u << pin);
        break;
#endif

#ifdef HAS_PORT_B
        case PORT_B:
            PORTB &= ~(1u << pin);
        break;
#endif

#ifdef HAS_PORT_C
        case PORT_C:
            PORTC &= ~(1u << pin);
        break;
#endif

#ifdef HAS_PORT_D
        case PORT_D:
            PORTD &= ~(1u << pin);
        break;
#endif

#ifdef HAS_PORT_E
        case PORT_E:
            PORTE &= ~(1u << pin);
        break;
#endif

#ifdef HAS_PORT_F
        case PORT_F:
            PORTF &= ~(1u << pin);
        break;
#endif

        default:

        break;
    }
}


void c_io::ddrxn_set(uint8_t port, uint8_t pin)
{
    switch(port)
    {
#ifdef HAS_PORT_A
        case PORT_A:
            DDRA |= (1u << pin);
        break;
#endif

#ifdef HAS_PORT_B
        case PORT_B:
            DDRB |= (1u << pin);
        break;
#endif

#ifdef HAS_PORT_C
        case PORT_C:
            DDRC |= (1u << pin);
        break;
#endif

#ifdef HAS_PORT_D
        case PORT_D:
            DDRD |= (1u << pin);
        break;
#endif

#ifdef HAS_PORT_E
        case PORT_E:
            DDRE |= (1u << pin);
        break;
#endif

#ifdef HAS_PORT_F
        case PORT_F:
            DDRF |= (1u << pin);
        break;
#endif

        default:

        break;
    }
}

void c_io::ddrxn_clear(uint8_t port, uint8_t pin)
{
    switch(port)
    {
#ifdef HAS_PORT_A
        case PORT_A:
            DDRA &= ~(1u << pin);
        break;
#endif

#ifdef HAS_PORT_B
        case PORT_B:
            DDRB &= ~(1u << pin);
        break;
#endif

#ifdef HAS_PORT_C
        case PORT_C:
            DDRC &= ~(1u << pin);
        break;
#endif

#ifdef HAS_PORT_D
        case PORT_D:
            DDRD &= ~(1u << pin);
        break;
#endif

#ifdef HAS_PORT_E
        case PORT_E:
            DDRE &= ~(1u << pin);
        break;
#endif

#ifdef HAS_PORT_F
        case PORT_F:
            DDRF &= ~(1u << pin);
        break;
#endif

        default:

        break;
    }
}


c_io::pin_type_t c_io::ddrxn_get(uint8_t port, uint8_t pin)
{
    uint8_t result = 0u;

    switch(port)
    {
#ifdef HAS_PORT_A
        case PORT_A:
            result = DDRA & (1u << pin);
        break;
#endif

#ifdef HAS_PORT_B
        case PORT_B:
            result = DDRB & (1u << pin);
        break;
#endif

#ifdef HAS_PORT_C
        case PORT_C:
            result = DDRC & (1u << pin);
        break;
#endif

#ifdef HAS_PORT_D
        case PORT_D:
            result = DDRD & (1u << pin);
        break;
#endif

#ifdef HAS_PORT_E
        case PORT_E:
            result = DDRE & (1u << pin);
        break;
#endif

#ifdef HAS_PORT_F
        case PORT_F:
            result = DDRF & (1u << pin);
        break;
#endif

        default:

        break;
    }

    return (c_io::pin_type_t)(result >> pin);
}


bool c_io::pinxn_get(uint8_t port, uint8_t pin)
{
    uint8_t result = 0u;

    switch(port)
    {
#ifdef HAS_PORT_A
        case PORT_A:
            result = PINA & (1u << pin);
        break;
#endif

#ifdef HAS_PORT_B
        case PORT_B:
            result = PINB & (1u << pin);
        break;
#endif

#ifdef HAS_PORT_C
        case PORT_C:
            result = PINC & (1u << pin);
        break;
#endif

#ifdef HAS_PORT_D
        case PORT_D:
            result = PIND & (1u << pin);
        break;
#endif

#ifdef HAS_PORT_E
        case PORT_E:
            result = PINE & (1u << pin);
        break;
#endif

#ifdef HAS_PORT_F
        case PORT_F:
            result = PINF & (1u << pin);
        break;
#endif

        default:

        break;
    }

    return (bool)result;
}
