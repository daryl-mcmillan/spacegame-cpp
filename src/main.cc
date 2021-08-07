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
    int lineCount = 8;
    Vector3 lines[][2] = {
        {Vector3(0,0,1), Vector3(0,100,1)},
        {Vector3(0,0,1), Vector3(0,-100,1)},
        {Vector3(0,0,1), Vector3(100,0,1)},
        {Vector3(0,0,1), Vector3(-100,0,1)},
        {Vector3(0,0,1), Vector3(100,100,1)},
        {Vector3(0,0,1), Vector3(100,-100,1)},
        {Vector3(0,0,1), Vector3(-100,-100,1)},
        {Vector3(0,0,1), Vector3(-100,100,1)},
    };

    NUMBER angle = 0.0;
    for( ;; ) {
        Buffer * buffer = display.getBuffer();
        buffer->clear(0);
        Matrix3 local = camera.mul(Matrix3::translate(150,0)).mul(Matrix3::rotate(angle));
        for( int i=0; i<lineCount; i++ ) {
            Vector3 p1 = local.mul(lines[i][0]);
            Vector3 p2 = local.mul(lines[i][1]);
            buffer->line(p1.x, p1.y, p2.x, p2.y, 1);
        }

        local = camera.mul(Matrix3::translate(-150,0)).mul(Matrix3::rotate(1-angle));
        for( int i=0; i<lineCount; i++ ) {
            Vector3 p1 = local.mul(lines[i][0]);
            Vector3 p2 = local.mul(lines[i][1]);
            buffer->line(p1.x, p1.y, p2.x, p2.y, 1);
        }

        display.releaseBuffer(buffer);
        //sleep_ms(5);
        angle = angle + 0.01;
    }

}

int main() {
	send();
	return 0;
}
