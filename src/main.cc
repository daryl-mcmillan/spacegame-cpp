#include "display.h"
#include "pico/stdlib.h"
#include "hardware/spi.h"

#define LED_PIN 25

void send() {
    Display display = Display::start();

    int x = 0;
    for( ;; ) {
        display.clear(0);
        for( int y=0; y<display.getHeight(); y++ ) {
            display.setPixel(x,y,255);
            display.setPixel(x-1,y,255);
            display.setPixel(x-2,y,255);
            display.setPixel(x-13,y,255);
            display.setPixel(x-14,y,255);
            display.setPixel(x-15,y,255);
        }
        display.line( x,0, 400-x, 240, 1 );
        display.line( 0,x, 400, 240-x, 1 );
        display.swap();
        sleep_ms(5);
        x = (x + 1) % (display.getWidth() + 20);
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
