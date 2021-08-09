#ifndef SPACESHIP_H_
#define SPACESHIP_H_

#include "../display.h"
#include "../matrix.h"

class Spaceship {
    Vector3 position;
    Vector3 velocity;
    NUMBER angle;
    bool booster;
    public:
        Spaceship(int x, int y);
        void update(Vector3 force);
        void draw(Matrix3 * transform, Buffer * buffer);
        Vector3 getPosition();
        void setBooster( bool val );
        bool getBooster();
        void stepRight();
        void stepLeft();
};

#endif