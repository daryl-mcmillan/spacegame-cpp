#include "pico/stdlib.h"
#include "hardware/spi.h"

#include "display.h"
#include "matrix.h"

#define LED_PIN 25

void send() {
    Display display = Display::start();

    Matrix3 camera = Matrix3::unit();
    camera = camera.mul(Matrix3::translate(200,120));
    camera = camera.mul(Matrix3::scale(0.5, -0.5));
    Vector3 p1(0,0,1);
    Vector3 p2(100,100,1);
    
    p1 = camera.mul(p1);
    p2 = camera.mul(p2);

    int x = 0;
    for( ;; ) {
        display.clear(0);
        display.line(p1.x, p1.y, p2.x, p2.y, 1);
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

int main() {
	send();
	return 0;
}
