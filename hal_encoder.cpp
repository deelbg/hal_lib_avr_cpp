#include <stdint.h>
#include "hal_avr_io.h"
#include "hal_encoder.h"


c_encoder::c_encoder(c_io & ref_io, bool use_raw_input, uint8_t encoder_pin_a, uint8_t encoder_pin_b) :
                    r_io(ref_io), use_raw(use_raw_input), pin_a(encoder_pin_a), pin_b(encoder_pin_b),
                    pulse_number_left(0u), pulse_number_right(0u)
{

}


void c_encoder::scan(void)
{
    static bool pin_a_high_old = false;
    bool pin_a_high = use_raw ? r_io.input_get_raw(pin_a) : r_io.input_get(pin_a);
    bool pin_b_high = use_raw ? r_io.input_get_raw(pin_b) : r_io.input_get(pin_b);

    if (pin_a_high && !pin_a_high_old)
    {
        if (pin_b_high)
        {
            if (0xFFFFu > pulse_number_left)
            {
                pulse_number_left++;
            }
        }
        else
        {
            if (0xFFFFu > pulse_number_right)
            {
                pulse_number_right++;
            }
        }

    }

    if (!pin_a_high && pin_a_high_old)
    {
        if (!pin_b_high)
        {
            if (0xFFFFu > pulse_number_left)
            {
                pulse_number_left++;
            }
        }
        else
        {
            if (0xFFFFu > pulse_number_right)
            {
                pulse_number_right++;
            }
        }

    }

    pin_a_high_old = pin_a_high;
}


uint8_t c_encoder::get_pulses_left(bool reset_pulses)
{
    uint8_t result = pulse_number_left;
    
    if (reset_pulses)
    {
        pulse_number_left = 0u;
    }

    return result;
}


uint8_t c_encoder::get_pulses_right(bool reset_pulses)
{
    uint8_t result = pulse_number_right;
    
    if (reset_pulses)
    {
        pulse_number_right = 0u;
    }

    return result;
}


void c_encoder::set_pulses_left(uint8_t pulses)
{
    pulse_number_left = pulses;
}


void c_encoder::set_pulses_right(uint8_t pulses)
{
    pulse_number_right = pulses;
}
