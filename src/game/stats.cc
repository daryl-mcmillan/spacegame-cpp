#include "stats.h"

#include <hardware/timer.h>

Stats::Stats() {
    lastCount = 0;
    count = 0;
    startTime = time_us_32();
}

void Stats::draw(Buffer * buffer) {
    count ++;
    uint32_t now = time_us_32();
    if( now - startTime >= 1000000 ) {
        lastCount = count;
        count = 0;
        startTime = now;
    }
    buffer->line(0,buffer->getHeight()-1,lastCount,buffer->getHeight()-1,1);
    buffer->setPixel(60,buffer->getHeight()-2,1);
    buffer->setPixel(60,buffer->getHeight()-3,1);
}
