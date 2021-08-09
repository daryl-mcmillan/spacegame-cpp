#include "spaceship.h"

//    Vector3 position;
//    Vector3 velocity;
//    NUMBER angle;
//    bool booster;

#define ANGLE_STEP 0.01;

Spaceship::Spaceship(int x, int y) {
    position = Vector3::point(x,y);
    velocity = Vector3::vector(0,0);
    angle = 0;
    booster = false;
}

void Spaceship::update(Vector3 force) {
    if( booster ) {
        Vector3 boost = Matrix3::rotate(angle).mul(Vector3::vector(0, 0.01));
        velocity = velocity.add(boost);
    }
    velocity = velocity.add(force);
    position = position.add(velocity);
}

void Spaceship::draw(Matrix3 * transform, Buffer * buffer) {
    Matrix3 local = transform->mul(Matrix3::translate(position.x, position.y));
    local = local.mul(Matrix3::rotate(angle));
    Vector3 top = local.mul(Vector3::point(0, 6));
    Vector3 left = local.mul(Vector3::point(-3,-2));
    Vector3 right = local.mul(Vector3::point(3,-2));
    buffer->line(top.x, top.y, left.x, left.y, 1);
    buffer->line(top.x, top.y, right.x, right.y, 1);
    if( booster ) {
        buffer->line(left.x, left.y, right.x, right.y, 1);
    }
}
Vector3 Spaceship::getPosition() {
    return position;
}
void Spaceship::setBooster( bool val ) {
    booster = val;
}
bool Spaceship::getBooster() {
    return booster;
}
void Spaceship::stepRight() {
    angle += ANGLE_STEP;
}
void Spaceship::stepLeft() {
    angle -= ANGLE_STEP;
}