#include <stdlib.h>
#include "pico/stdlib.h"
#include "hardware/spi.h"

#include "display.h"
#include "matrix.h"
#include "math.h"

#include "game/planet.h"

void send() {
    srand(1);
    
    Display * display = Display::start();

    Matrix3 camera = Matrix3::unit();
    camera = camera.mul(Matrix3::translate(200,120));
    camera = camera.mul(Matrix3::scale(0.8, -0.8));
    
    Planet planet(50,50);

    NUMBER angle = 0.0;
    for( ;; ) {
        Buffer * buffer = display->getDrawingBuffer();

        buffer->clear(0);
        Matrix3 local = camera.mul(Matrix3::rotate(angle));
        planet.draw( &local, buffer);

        display->releaseDrawingBuffer(buffer);
        sleep_ms(3);
        angle = angle + 0.001;
    }

}

int main() {
	send();
	return 0;
}
