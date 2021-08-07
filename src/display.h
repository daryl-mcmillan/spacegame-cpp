#include "pico/stdlib.h"
#include "pico/multicore.h"

class Buffer {
    uint8_t * rawBuffer;
    uint8_t * screen;
    public:
        Buffer();
        const int getWidth() const;
        const int getHeight() const;
        void clear( uint8_t val );
        void setPixel( int x, int y, uint8_t val );
        void line( int x1, int y1, int x2, int y2, uint8_t val );
};

class Display {
    uint8_t * volatile buffer;
    Buffer b;
    public:
        static Display start();
        uint16_t getWidth();
        uint16_t getHeight();
        void setPixel( int x, int y, uint8_t val );
        void line( int x1, int y1, int x2, int y2, uint8_t val );
        void clear( uint8_t val );
        void swap();
};
