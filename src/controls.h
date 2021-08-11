#ifndef CONTROLS_H_
#define CONTROLS_H_

#include <stdlib.h>
#include "pico/stdlib.h"

#define DIRECTION uint8_t
#define DIRECTION_UP 0b01
#define DIRECTION_DOWN 0b11
#define DIRECTION_LEFT 0b100
#define DIRECTION_RIGHT 0b1100

class Controls {
    public:
        Controls();
        DIRECTION getDirection();
        bool getAButton();
        bool getBButton();
};

#endif
