#include "display.h"
#include "pico/stdlib.h"
#include "hardware/spi.h"

#define LED_PIN 25

void send() {
    Display display = Display::start();

    int x = 0;
    for( ;; ) {
        for( int i=0; i<100; i++ ) {
            display.setPixel(i+x,i,255);
        }
        for( int i=0; i<100; i++ ) {
            display.setPixel(10+i,i,255);
        }
        sleep_ms(5);
        //display.send();
        x++;
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
