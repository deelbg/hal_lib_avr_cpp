#include <stdint.h>
#include "hal_at24c0x_eeprom.h"


c_at24c0x_eeprom::c_at24c0x_eeprom(c_i2c & ref_i2c, uint8_t i2c_address, chip_type_t type) :
                                   r_i2c(ref_i2c), chip_address(i2c_address), chip_type(type)
{

}


uint8_t c_at24c0x_eeprom::i2c_address(uint16_t address)
{
    uint8_t tmp_address = 0x00u;

    switch (chip_type)
    {
        case AT24C04:
            tmp_address = (chip_address & 0x00FEu) | ((address >> 8u) & 0x01u);
        break;

        case AT24C08:
            tmp_address = (chip_address & 0x00FCu) | ((address >> 8u) & 0x03u);
        break;

        case AT24C16:
            tmp_address = (chip_address & 0x00F8u) | ((address >> 8u) & 0x07u);
        break;

        case AT24C01:
        case AT24C02:
        default:            
            tmp_address = chip_address & 0x00FFu;
        break;
    }

    return tmp_address;
}


uint8_t c_at24c0x_eeprom::read_byte(uint16_t address)
{
    uint8_t result = 0x00u;

    r_i2c.start_write(i2c_address(address));
    r_i2c.write_byte(address & 0x00FFu);
    r_i2c.start_read(i2c_address(address));
    result = r_i2c.read_byte(c_i2c::NO_ACK);
    r_i2c.stop();

    return result;
}


void c_at24c0x_eeprom::write_byte(uint16_t address, uint8_t byte)
{
    r_i2c.start_write(i2c_address(address));
    r_i2c.write_byte(address & 0x00FFu);
    r_i2c.write_byte(byte);
    r_i2c.stop();
}


void c_at24c0x_eeprom::read_block(uint8_t * const block, uint16_t address, uint16_t size)
{
    for (uint16_t index = 0; index < size; index++)
    {
        *(block + index) = read_byte(address + index);
    }
}


void c_at24c0x_eeprom::write_block(uint8_t const * const block, uint16_t address, uint16_t size)
{
    for (uint16_t index = 0; index < size; index++)
    {
        write_byte((address + index), *(block + index));
    }
}
