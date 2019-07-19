#include <stdint.h>
#include "timer.h"

c_timer::c_timer() : counter(0)
{

}


void c_timer::start(uint16_t value)
{
    counter = value;
}


void c_timer::decrement(void)
{
    if (0u != counter)
    {
        counter--;
    }
}


bool c_timer::expired(void)
{
    return !((bool)counter);
}        
