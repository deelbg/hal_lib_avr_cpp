#include <stdint.h>
#include <avr/eeprom.h>
#include "hal_avr_eeprom.h"


c_eeprom::c_eeprom()
{

}


uint8_t c_eeprom::read_byte(uint16_t address)
{    
    eeprom_busy_wait();
    return eeprom_read_byte((uint8_t *)address);
}


void c_eeprom::write_byte(uint16_t address, uint8_t byte)
{    
    eeprom_busy_wait();
    eeprom_write_byte((uint8_t *)address, byte);
}


void c_eeprom::read_block(uint8_t * const block, uint16_t address, uint16_t size)
{
    for (uint16_t index = 0; index < size; index++)
    {
        *(block + index) = read_byte(address + index);
    }
}


void c_eeprom::write_block(uint8_t const * const block, uint16_t address, uint16_t size)
{
    for (uint16_t index = 0; index < size; index++)
    {
        write_byte((address + index), *(block + index));
    }
}
