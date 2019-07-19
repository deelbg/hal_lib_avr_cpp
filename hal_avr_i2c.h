#ifndef I2C_H
#define I2C_H

#define I2C_VERSION_MAJOR 1
#define I2C_VERSION_MINOR 0
#define I2C_VERSION_PATCH 0

// NOTE: Adjust the CPU frequency.
#define F_CPU 4000000ul
// Uncommment for 400kHz. Leve commented out for 100kHz.
#define I2C_FAST_MODE_400KHZ
// Uncommment for USI hardware. Leve commented out for TWI.
#define I2C_HARDWARE_USI

#ifdef I2C_HARDWARE_USI
#define I2C_PINS
#define USI_DDR               DDRB
#define USI_SCL_DDA           DDB2
#define USI_SDA_DDA           DDB0

#define USI_PORT              PORTB
#define USI_SCL_PORT_PIN      PORTB2
#define USI_SDA_PORT_PIN      PORTB0

#define USI_PIN               PINB
#define USI_SCL_PIN           PINB2
#define USI_SDA_PIN           PINB0
#endif // I2C_HARDWARE_USI

class c_i2c
{
    public:
        enum i2c_receive_response_t
        {
            ACK,
            NACK
        };

        c_i2c();
        bool start_write(uint8_t slave_address);
        bool start_read(uint8_t slave_address);
        void stop(void);
        bool write_byte(uint8_t byte);
        uint8_t read_byte(i2c_receive_response_t send_ack); 
                
    private:
        void init(void);

#ifdef I2C_HARDWARE_USI
        void send_start(void);
        uint8_t usi_transfer(uint8_t data_to_send, uint8_t bit_number);
#else // I2C_HARDWARE_USI
        void transfer_begin(uint8_t slave_address);
        uint8_t twi_status(void);
#endif // I2C_HARDWARE_USI

};

#endif //I2C_H
