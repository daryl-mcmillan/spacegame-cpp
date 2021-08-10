#include <stdlib.h>
#include "pico/stdlib.h"
#include "hardware/spi.h"

#include "display.h"
#include "matrix.h"
#include "math.h"

#include "game/planet.h"
#include "game/spaceship.h"
#include "game/stats.h"

void send() {
    srand(1);
    
    Display * display = Display::start();

    Matrix3 camera = Matrix3::unit();
    camera = camera.mul(Matrix3::translate(200,120));
    camera = camera.mul(Matrix3::scale(0.8, -0.8));
    
    Stats stats;
    Planet planet(50,50);
    Spaceship ship(50,50);

    ship.setBooster(true);
    
    NUMBER angle = 0.0;
    int shake = 0;
    for( ;; ) {
        Matrix3 local = camera.mul(Matrix3::rotate(angle));
        if( ship.getBooster() ) {
            shake ++;
            int displaceX = (( rand() & 255 ) - 127 ) * shake * 0.0001;
            int displaceY = (( rand() & 255 ) - 127 ) * shake * 0.0001;
            local = local.mul(Matrix3::translate(displaceX,displaceY));
        } else {
            shake = 0;
        }
        
        ship.stepLeft();
        ship.update(Vector3::vector(0,0));

        Buffer * buffer = display->getDrawingBuffer();

        buffer->clear(0);
        planet.draw(&local, buffer);
        ship.draw(&local, buffer);
        stats.draw(buffer);

        display->releaseDrawingBuffer(buffer);
        //sleep_ms(3);
        angle = angle + 0.001;
    }

}

int main() {
	send();
	return 0;
}
