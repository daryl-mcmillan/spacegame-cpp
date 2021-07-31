#include "matrix.h"

Vector3 Vector3::vector(int x, int y) {
  return Vector3(x,y,0);
}
Vector3 Vector3::point(int x, int y) {
  return Vector3(x,y,1);
}

Matrix3 Matrix3::mul(Matrix3 other) {
  return Matrix3(
    m11 * other.m11 + m12 * other.m21 + m13 * other.m31,
    m11 * other.m12 + m12 * other.m22 + m13 * other.m32,
    m11 * other.m13 + m12 * other.m23 + m13 * other.m33,

    m21 * other.m11 + m22 * other.m21 + m23 * other.m31,
    m21 * other.m12 + m22 * other.m22 + m23 * other.m32,
    m21 * other.m13 + m22 * other.m23 + m23 * other.m33,

    m31 * other.m11 + m32 * other.m21 + m33 * other.m31,
    m31 * other.m12 + m32 * other.m22 + m33 * other.m32,
    m31 * other.m13 + m32 * other.m23 + m33 * other.m33
  );
}
Vector3 Matrix3::mul(Vector3 other) {
  return Vector3(
    m11 * other.x + m12 * other.y + m13 * other.isPoint,
    m21 * other.x + m22 * other.y + m23 * other.isPoint,
    m31 * other.x + m32 * other.y + m33 * other.isPoint
  );
}
Matrix3 Matrix3::unit() {
  return Matrix3(
    1, 0, 0,
    0, 1, 0,
    0, 0, 1
  );
}
Matrix3 Matrix3::rotate(int angle) {
  return Matrix3::unit();
}
Matrix3 Matrix3::translate(Vector3 delta) {
  return Matrix3(
    1, 0, delta.x,
    0, 1, delta.y,
    0, 0, 1
  );
}
Matrix3 Matrix3::scale(Vector3 unit) {
  return Matrix3(
    unit.x, 0, 0,
    0, unit.y, 0,
    0, 0, 1
  );
}
