#include "pico/stdlib.h"
#include "pico/multicore.h"

class Display {
    uint8_t * volatile buffer;
    public:
        static Display start();
        uint16_t getWidth();
        uint16_t getHeight();
        void setPixel( uint16_t x, uint16_t y, uint8_t val );
        void clear( uint8_t val );
        void swap();
};
