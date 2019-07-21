#ifndef A24CXX_EEPROM_H
#define A24CXX_EEPROM_H

#include "hal_avr_i2c.h"

#define A24CXX_EEPROM_VERSION_MAJOR 1
#define A24CXX_EEPROM_VERSION_MINOR 0
#define A24CXX_EEPROM_VERSION_PATCH 0

class c_at24cxx_eeprom
{
    public:
        enum chip_type_t
        {
            AT24C01 = 0,
            AT24C02,
            AT24C04,
            AT24C08,
            AT24C16,
            AT24C32,
            AT24C64
        };

        c_at24cxx_eeprom(c_i2c & ref_i2c, uint8_t i2c_address, chip_type_t type);

        uint8_t read_byte(uint16_t address);
        void write_byte(uint16_t address, uint8_t byte);

        void read_block(uint8_t * const block, uint16_t address, uint16_t size);
        void write_block(uint8_t const * const block, uint16_t address, uint16_t size);
        
    private:
        chip_type_t chip_type;
        uint8_t chip_address;
        c_i2c & r_i2c;

        uint8_t i2c_address(uint16_t address);
};

#endif //A24C0X_EEPROM_H
