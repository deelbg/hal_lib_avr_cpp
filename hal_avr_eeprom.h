#ifndef AVR_EEPROM_H
#define AVR_EEPROM_H

#define EEPROM_VERSION_MAJOR 1
#define EEPROM_VERSION_MINOR 0
#define EEPROM_VERSION_PATCH 0

class c_eeprom
{
    public:
        c_eeprom();
        uint8_t read_byte(uint16_t address);
        void write_byte(uint16_t address, uint8_t byte);

        void read_block(uint8_t * const block, uint16_t address, uint16_t size);
        void write_block(uint8_t const * const block, uint16_t address, uint16_t size);
        
    private:

};

#endif //AVR_EEPROM_H
