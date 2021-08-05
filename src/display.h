#include "pico/stdlib.h"

class Display {
    uint8_t * buffer;
    public:
        Display();
        void setPixel( uint16_t x, uint16_t y, uint8_t val );
        void send();
};
