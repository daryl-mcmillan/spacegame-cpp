#include <stdlib.h>
#include "pico/stdlib.h"
#include "hardware/spi.h"

#include "display.h"
#include "matrix.h"
#include "math.h"

#define LED_PIN 25

void send() {
    srand(1);
    
    Display * display = Display::start();

    Matrix3 camera = Matrix3::unit();
    camera = camera.mul(Matrix3::translate(200,120));
    camera = camera.mul(Matrix3::scale(0.5, -0.5));
    int lineCount = 40;
    Vector3 points[lineCount];
    for( int i=0; i<lineCount; i++) {
        double lineAngle = M_PI * 2.0 * (double)i / (double)lineCount;
        double length = 80 + (rand()%30);
        points[i] = Matrix3::rotate(lineAngle).mul(Vector3(0,length,1));
    }

    NUMBER angle = 0.0;
    for( ;; ) {
        Buffer * buffer = display->getDrawingBuffer();
        buffer->clear(0);
        Matrix3 local = camera.mul(Matrix3::rotate(angle));
        for( int i=0; i<lineCount; i++ ) {
            Vector3 p1 = local.mul(points[i]);
            Vector3 p2 = local.mul(points[(i+1)%lineCount]);
            buffer->line(p1.x, p1.y, p2.x, p2.y, 1);
        }

        display->releaseDrawingBuffer(buffer);
        sleep_ms(3);
        angle = angle + 0.01;
    }

}

int main() {
	send();
	return 0;
}
