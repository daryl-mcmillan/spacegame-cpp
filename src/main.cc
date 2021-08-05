#include "display.h"
#include "pico/stdlib.h"
#include "hardware/spi.h"

#define LED_PIN 25

void send() {
    Display display;

    for( ;; ) {
        for( int i=0; i<100; i++ ) {
            display.setPixel(i,i,255);
        }
        display.send();
        
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
