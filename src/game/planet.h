#include "../matrix.h"
#include "../display.h"

#ifndef PLANET_H_
#define PLANET_H_

class Planet {
    Vector3 position;
    Vector3 * points;
    int numPoints;
    public:
        Planet(int x, int y);
        void draw( Matrix3 * transform, Buffer * buffer );
        Vector3 getPosition();
        NUMBER getRadius();
};

#endif
