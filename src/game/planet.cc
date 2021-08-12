#include "planet.h"

#include "math.h"
#include <stdlib.h>

#define BASE_RADIUS 80
#define RANDOM_EXTRA 30

Planet::Planet(int x, int y) {
    position = Vector3(x,y,1);
    numPoints = 100;
    points = new Vector3[numPoints];
    for( int i=0; i<numPoints; i++) {
        double lineAngle = M_PI * 2.0 * (double)i / (double)numPoints;
        double length = BASE_RADIUS + (rand() % RANDOM_EXTRA);
        points[i] = Matrix3::rotate(lineAngle).mul(Vector3(0,length,1));
    }
}

void Planet::draw( Matrix3 * transform, Buffer * buffer ) {
    Matrix3 local = transform->mul(Matrix3::translate(position.x, position.y));
    Vector3 p2 = local.mul(points[numPoints-1]);
    for( int i=0; i<numPoints;i++) {
        Vector3 p1 = local.mul(points[i]);
        buffer->line(p1.x, p1.y, p2.x, p2.y, 1);
        p2 = p1;
    }
}

Vector3 Planet::getPosition() {
    return position;
}

NUMBER Planet::getRadius() {
    return BASE_RADIUS + RANDOM_EXTRA / 2;
}
