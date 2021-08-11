#include "controls.h"

#include <stdlib.h>
#include "pico/stdlib.h"
#include "hardware/gpio.h"

#define A_BUTTON_PIN 2
#define B_BUTTON_PIN 3
#define UP_BUTTON_PIN 4
#define DOWN_BUTTON_PIN 5
#define LEFT_BUTTON_PIN 6
#define RIGHT_BUTTON_PIN 7

void configurePin(uint pin) {
    gpio_init(pin);
    gpio_set_dir(pin, GPIO_IN);
    gpio_pull_up(pin);
}

Controls::Controls() {
    configurePin(A_BUTTON_PIN);
    configurePin(B_BUTTON_PIN);
    configurePin(UP_BUTTON_PIN);
    configurePin(DOWN_BUTTON_PIN);
    configurePin(LEFT_BUTTON_PIN);
    configurePin(RIGHT_BUTTON_PIN);
}

DIRECTION Controls::getDirection() {
    return 0;
}

bool Controls::getAButton() {
    if( gpio_get(A_BUTTON_PIN) ) {
        return false;
    } else {
        return true;
    }
}

bool Controls::getBButton() {
    if( gpio_get(B_BUTTON_PIN) ) {
        return false;
    } else {
        return true;
    }
}
