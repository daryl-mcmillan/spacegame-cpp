#include <stdlib.h>
#include "pico/stdlib.h"
#include "hardware/spi.h"

#include "controls.h"
#include "display.h"
#include "matrix.h"
#include "math.h"

#include "game/planet.h"
#include "game/spaceship.h"
#include "game/stats.h"

void send() {
    srand(1);
    
    Controls controls;
    Display * display = Display::start();

    Matrix3 camera = Matrix3::unit();
    camera = camera.mul(Matrix3::translate(200,120));
    camera = camera.mul(Matrix3::scale(0.8, -0.8));
    
    Stats stats;
    Planet planet(50,50);
    Spaceship ship(50,50);
    
    NUMBER angle = 0.0;
    int shake = 0;
    for( ;; ) {
        ship.setBooster(controls.getAButton());
        Matrix3 local = camera.mul(Matrix3::rotate(angle));

        Vector3 shipToWorld = planet.getPosition().sub(ship.getPosition());
        NUMBER distanceToWorld = shipToWorld.getLength();
        //var distanceToWorld = len_v3(shipToWorld);

        //var g = [0,0,0];
        //if( distanceToWorld > 0 ) {
        //    var unitScale = 1 / distanceToWorld;
        //    var unit = mul_v3_v3( shipToWorld, [unitScale, unitScale, 0] );
        //    g = mul_v3_v3( unit, [0.01, 0.01, 0] );
        //}
        //var lookAheadDistance = ( distanceToWorld - world.radius - 50 ) * 0.6;
        //if( distanceToWorld > 0 ) {
            //var lookAheadFraction = lookAheadDistance / distanceToWorld;
            //var lookAheadVector = mul_v3_v3( shipToWorld, [lookAheadFraction, lookAheadFraction, 0] );
            //var poi = add_v3_v3( ship.p, lookAheadVector );
            //camera.p = poi;
        //}
        NUMBER cameraScale = 180 / fmax(distanceToWorld - 150,100);
        local = local.mul(Matrix3::scale(cameraScale, cameraScale));

        if( ship.getBooster() ) {
            shake ++;
            int displaceX = (( rand() & 255 ) - 127 ) * shake * 0.0001;
            int displaceY = (( rand() & 255 ) - 127 ) * shake * 0.0001;
            local = local.mul(Matrix3::translate(displaceX,displaceY));
        } else {
            shake = 0;
        }
        
        //ship.stepLeft();
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
