#ifndef ENCODER_H
#define ENCODER_H

#define ENCODER_VERSION_MAJOR 1
#define ENCODER_VERSION_MINOR 0
#define ENCODER_VERSION_PATCH 0


class c_encoder  
{        
    public:
        c_encoder(c_io & ref_io, bool use_raw_input, uint8_t encoder_pin_a, uint8_t encoder_pin_b);

        void scan(void);
        uint8_t get_pulses_left(bool reset_pulses);
        uint8_t get_pulses_right(bool reset_pulses);
        void set_pulses_left(uint8_t pulses);
        void set_pulses_right(uint8_t pulses);

    private:
        c_io  & r_io;
        bool use_raw;
        uint8_t pin_a;
        uint8_t pin_b;

        uint8_t pulse_number_left;
        uint8_t pulse_number_right;
};


#endif //ENCODER_H
