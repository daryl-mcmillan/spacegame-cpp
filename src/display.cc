#include "display.h"

#include "pico/stdlib.h"
#include "hardware/spi.h"
#include "string.h"

#define SHARPMEM_BIT_WRITECMD (0b10000000) // for MSB first output
#define SHARPMEM_BIT_VCOM (0b01000000)     // for MSB first output
#define SHARPMEM_BIT_CLEAR (0b00100000)    // for MSB first output

#define RAW_BUFFER_DATA_START 2
#define ROWS 240
#define COLS 400
#define ROW_BYTES 50
#define ROW_STRIDE 52
#define BUFFER_LENGTH (52*ROWS+2)

uint8_t flipBits(uint8_t val) {
    uint8_t result = 0;
    for( int i=0; i<8; i++ ) {
        result = (result<<1) | (val & 1 );
        val = val >> 1;
    }
    return result;
}

uint8_t * createBuffer() {
    uint8_t * buffer = new uint8_t[BUFFER_LENGTH];
    for( int i=0; i<BUFFER_LENGTH; i++ ) {
        buffer[i] = 0;
    }
    buffer[0] = SHARPMEM_BIT_WRITECMD;
    for( int line=0; line<ROWS; line++ ) {
        buffer[line * ROW_STRIDE + 1] = flipBits( line + 1 );
    }
    return buffer;
}

Buffer::Buffer() {
    rawBuffer = createBuffer();
    screen = rawBuffer + RAW_BUFFER_DATA_START;
}
const int Buffer::getWidth() const {
    return COLS;
}
const int Buffer::getHeight() const {
    return ROWS;
}
void Buffer::clear( uint8_t val ) {
    if( val ) {
        val = 0xff;
    }
    const uint8_t * afterLastRow = screen + ROWS * ROW_STRIDE;
    for( uint8_t * rowStart = screen; rowStart < afterLastRow; rowStart += ROW_STRIDE ) {
        memset(rowStart, val, ROW_BYTES );
    }
}
void Buffer::setPixel( int x, int y, uint8_t val ) {
    int xByte = x >> 3; // shift first for a cheaper bounds check
    if( xByte < 0 || xByte >= ROW_BYTES || y < 0 || y >= ROWS ) {
        return;
    }
    uint8_t bit = 128 >> (x & 0b111);
    int index = y * ROW_STRIDE + xByte;
    if( val ) {
        screen[index] |= bit;
    } else {
        screen[index] &= (~bit);
    }
}
void Buffer::line( int x1, int y1, int x2, int y2, uint8_t val ) {
    int stepX = 1;
    int dx = x2 - x1;
    if( dx < 0 ) {
        stepX = -1;
        dx = -dx;
    }
    int stepY = 1;
    int dy = y2 - y1;
    if( dy < 0 ) {
        stepY = -1;
        dy = -dy;
    }
    int x = x1;
    int y = y1;
    if( dx >= dy ) {
        int d = 2 * dy - dx;
        while(1) {
            setPixel(x,y,val);
            if( d > 0 ) {
                y = y + stepY;
                d = d - 2 * dx;
            }
            d = d + 2 * dy;
            if( x == x2 ) {
                break;
            }
            x = x + stepX;
        }
    } else {
        int d = 2 * dx - dy;
        while(1) {
            setPixel(x,y,val);
            if( d > 0 ) {
                x = x + stepX;
                d = d - 2 * dy;
            }
            d = d + 2 * dx;
            if( y == y2 ) {
                break;
            }
            y = y + stepY;
        }
    }
}
uint8_t * Buffer::getCommandBuffer() {
    return rawBuffer;
}

void send_all_helper(uint8_t vcomBit, uint8_t * buffer) {
    buffer[0] = SHARPMEM_BIT_WRITECMD | vcomBit;
    gpio_put(PICO_DEFAULT_SPI_CSN_PIN, 1);
    sleep_us(3);
    spi_write_blocking(spi_default, buffer, BUFFER_LENGTH);
    sleep_us(1);
    gpio_put(PICO_DEFAULT_SPI_CSN_PIN, 0);
}
void send_keep_helper( uint8_t vcomBit ) {
    uint8_t buffer[2];
    buffer[0] = vcomBit;
    gpio_put(PICO_DEFAULT_SPI_CSN_PIN, 1);
    sleep_us(3);
    spi_write_blocking(spi_default, buffer, 2);
    sleep_us(1);
    gpio_put(PICO_DEFAULT_SPI_CSN_PIN, 0);    
}

static uint8_t refreshCounter;
static Display * display;
void refreshThread() {
    for( ;; ) {
        refreshCounter++;
        uint8_t vcomBit = ( refreshCounter & 0b100 ) ? SHARPMEM_BIT_VCOM : 0;
        Buffer * buffer = display->getSendBuffer();
        if( buffer ) {
            send_all_helper( vcomBit, buffer->getCommandBuffer() );
            display->releaseSendBuffer(buffer);
        } else {
            send_keep_helper( vcomBit );
        }
        sleep_ms(15);
    }
}

void refreshSetup(Display * d) {
    refreshCounter = 0;
    display = d;
}

Display * Display::start() {
    Display * result = new Display();
    critical_section_init ( &(result->lock) );
    result->available1 = new Buffer();
    result->available2 = new Buffer();
    result->pending = NULL;

    spi_init(spi_default, 8000 * 1000);
    gpio_set_function(PICO_DEFAULT_SPI_RX_PIN, GPIO_FUNC_SPI);
    gpio_set_function(PICO_DEFAULT_SPI_SCK_PIN, GPIO_FUNC_SPI);
    gpio_set_function(PICO_DEFAULT_SPI_TX_PIN, GPIO_FUNC_SPI);    

    gpio_init(PICO_DEFAULT_SPI_CSN_PIN);
    gpio_set_dir(PICO_DEFAULT_SPI_CSN_PIN, GPIO_OUT);
    gpio_put(PICO_DEFAULT_SPI_CSN_PIN, 0);

    // active high chip select
    gpio_put(PICO_DEFAULT_SPI_CSN_PIN, 0);

    refreshSetup(result);
    multicore_launch_core1(refreshThread);
    return result;
}

Buffer * Display::getDrawingBuffer() {
    critical_section_enter_blocking(&lock);
    Buffer * ret = NULL;
    if( available2 ) {
        ret = available2;
        available2 = NULL;
    } else if( available1 ) {
        ret = available1;
        available1 = NULL;
    } else if( pending ) {
        ret = pending;
        pending = NULL;
    }
    critical_section_exit(&lock);
    return ret;
}

void Display::releaseDrawingBuffer(Buffer * buffer) {
    critical_section_enter_blocking(&lock);
    if( pending ) {
        available1 = pending;
        pending = buffer;
    } else {
        pending = buffer;
    }
    critical_section_exit(&lock);
}

Buffer * Display::getSendBuffer() {
    critical_section_enter_blocking(&lock);
    Buffer * ret = pending;
    pending = NULL;
    critical_section_exit(&lock);
    return ret;
}

void Display::releaseSendBuffer(Buffer * buffer) {
    critical_section_enter_blocking(&lock);
    if( available1 == NULL ) {
        available1 = buffer;
    } else if( available2 == NULL ) {
        available2 = buffer;
    }
    critical_section_exit(&lock);
}
