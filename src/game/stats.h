#ifndef STATS_H_
#define STATS_H_

#include "../display.h"

class Stats {
    uint32_t lastCount;
    uint32_t count;
    uint32_t startTime;
    public:
        Stats();
        void draw(Buffer * buffer);
};

#endif
