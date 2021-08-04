#include "pico/stdlib.h"
#include "hardware/spi.h"

const uint LED_PIN = 25;
//#define SHARPMEM_BIT_WRITECMD (0x80) // 0x80 in MSB format
//#define SHARPMEM_BIT_VCOM (0x40)     // 0x40 in MSB format
//#define SHARPMEM_BIT_CLEAR (0x20)    // 0x20 in MSB format

#define SHARPMEM_BIT_WRITECMD (0b10000000) // 0x80 in LSB format
#define SHARPMEM_BIT_VCOM (0b01000000)     // 0x40 in LSB format
#define SHARPMEM_BIT_CLEAR (0b00100000)    // 0x20 in LSB format

void send() {
    spi_init(spi_default, 1000 * 1000);
    gpio_set_function(PICO_DEFAULT_SPI_RX_PIN, GPIO_FUNC_SPI);
    gpio_set_function(PICO_DEFAULT_SPI_SCK_PIN, GPIO_FUNC_SPI);
    gpio_set_function(PICO_DEFAULT_SPI_TX_PIN, GPIO_FUNC_SPI);    

    gpio_init(PICO_DEFAULT_SPI_CSN_PIN);
    gpio_set_dir(PICO_DEFAULT_SPI_CSN_PIN, GPIO_OUT);
    gpio_put(PICO_DEFAULT_SPI_CSN_PIN, 0);

    //gpio_init(LED_PIN);
    //gpio_set_dir(LED_PIN, GPIO_OUT);
    
    uint8_t command = SHARPMEM_BIT_WRITECMD;
    uint8_t pixel = 0;
    for( ;; ) {
        // active high chip select
        sleep_ms(1);
        gpio_put(PICO_DEFAULT_SPI_CSN_PIN, 1);
        sleep_ms(1);

        // send data
        uint8_t buf[54];
        buf[0] = command;
        buf[1] = 0b10000000; // line number
        for( int i=0; i<50; i++ ) {
            buf[2+i] = pixel;
        }
        buf[52] = 0;
        buf[53] = 0;
        spi_write_blocking(spi_default, buf, 54);
        command = command ^ SHARPMEM_BIT_VCOM;
        pixel = pixel + 1;

        // clear chip select
        sleep_ms(1);
        gpio_put(PICO_DEFAULT_SPI_CSN_PIN, 0);
        sleep_ms(1);
        
        sleep_ms(100);
        //gpio_put(LED_PIN, pinval );
    }

}

int blink() {

    gpio_init(LED_PIN);
    gpio_set_dir(LED_PIN, GPIO_OUT);
    while (1) {
        gpio_put(LED_PIN, 0);
        sleep_ms(400);
        gpio_put(LED_PIN, 1);
        sleep_ms(400);
    }
}

int main() {
	send();
	return 0;
}
