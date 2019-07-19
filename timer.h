#ifndef TIMER_H
#define TIMER_H

#define TIMER_VERSION_MAJOR 1
#define TIMER_VERSION_MINOR 0
#define TIMER_VERSION_PATCH 0

class c_timer
{
    public:
        c_timer();
        
        void start(uint16_t value);
        void decrement(void);
        bool expired(void);
        
    private:
        volatile uint16_t counter;
};

#endif //TIMER_H
