#include "display.h"

#include "pico/stdlib.h"
#include "hardware/spi.h"

#define SHARPMEM_BIT_WRITECMD (0b10000000) // for MSB first output
#define SHARPMEM_BIT_VCOM (0b01000000)     // for MSB first output
#define SHARPMEM_BIT_CLEAR (0b00100000)    // for MSB first output

#define ROWS 240
#define COLS 400
#define COL_BYTES 52
#define BUFFER_LENGTH (52*ROWS+2)

void send_helper(uint8_t * buffer) {
    gpio_put(PICO_DEFAULT_SPI_CSN_PIN, 1);
    sleep_ms(1);
    buffer[0] = buffer[0] ^ SHARPMEM_BIT_VCOM;
    spi_write_blocking(spi_default, buffer, BUFFER_LENGTH);
    sleep_ms(1);
    gpio_put(PICO_DEFAULT_SPI_CSN_PIN, 0);
}

static uint8_t * refreshBuffer;
void refreshThread() {
    uint8_t * buffer = refreshBuffer;
    for( ;; ) {
        send_helper(buffer);
        sleep_ms(16);
    }
}

uint8_t flipBits(uint8_t val) {
    uint8_t result = 0;
    for( int i=0; i<8; i++ ) {
        result = (result<<1) | (val & 1 );
        val = val >> 1;
    }
    return result;
}

Display Display::start() {
    Display result;
    result.buffer = new uint8_t[BUFFER_LENGTH];
    for( int i=0; i<BUFFER_LENGTH; i++ ) {
        result.buffer[i] = 0;
    }
    result.buffer[0] = SHARPMEM_BIT_WRITECMD;
    for( int line=0; line<ROWS; line++ ) {
        result.buffer[line * COL_BYTES + 1] = flipBits( line + 1 );
    }

    spi_init(spi_default, 2000 * 1000);
    gpio_set_function(PICO_DEFAULT_SPI_RX_PIN, GPIO_FUNC_SPI);
    gpio_set_function(PICO_DEFAULT_SPI_SCK_PIN, GPIO_FUNC_SPI);
    gpio_set_function(PICO_DEFAULT_SPI_TX_PIN, GPIO_FUNC_SPI);    

    gpio_init(PICO_DEFAULT_SPI_CSN_PIN);
    gpio_set_dir(PICO_DEFAULT_SPI_CSN_PIN, GPIO_OUT);
    gpio_put(PICO_DEFAULT_SPI_CSN_PIN, 0);

    // active high chip select
    gpio_put(PICO_DEFAULT_SPI_CSN_PIN, 0);

    refreshBuffer = result.buffer;
    multicore_launch_core1(refreshThread);
    return result;
}

uint16_t Display::getWidth() {
    return COLS;
}

uint16_t Display::getHeight() {
    return ROWS;
}

void Display::setPixel(uint16_t x, uint16_t y, uint8_t val) {
    if( x < 0 || x >= COLS || y < 0 || y >= ROWS ) {
        return;
    }
    uint8_t bit = 128 >> (x & 0b111);
    x = x >> 3;
    int index = y * COL_BYTES + x + 2;
    if( val ) {
        buffer[index] = buffer[index] | bit;
    } else {
        buffer[index] = buffer[index] & (~bit);
    }
}
