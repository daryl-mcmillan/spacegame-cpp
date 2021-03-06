#include "pico/stdlib.h"
#include "pico/multicore.h"
#include "pico/critical_section.h"

#ifndef DISPLAY_H_
#define DISPLAY_H_

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
        uint8_t * getCommandBuffer();
};

class Display {
    critical_section_t lock;
    Buffer * volatile pending;
    Buffer * volatile available1;
    Buffer * volatile available2;
    int dmaChannel;
    public:
        static Display * start();
        Buffer * getDrawingBuffer();
        void releaseDrawingBuffer(Buffer * buffer);
        Buffer * getSendBuffer();
        void releaseSendBuffer(Buffer * buffer);
        void sendThread();
};

#endif