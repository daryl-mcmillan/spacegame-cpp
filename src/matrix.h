#define NUMBER float

class Vector3 {
  public:
    NUMBER x;
    NUMBER y;
    NUMBER isPoint;
    Vector3(NUMBER _x, NUMBER _y, NUMBER _isPoint) {
      x = _x;
      y = _y;
      isPoint = _isPoint;
    }
    static Vector3 vector(NUMBER x, NUMBER y);
    static Vector3 point(NUMBER x, NUMBER y);
};

class Matrix3 {
  public:
    NUMBER m11;
    NUMBER m12;
    NUMBER m13;
    NUMBER m21;
    NUMBER m22;
    NUMBER m23;
    NUMBER m31;
    NUMBER m32;
    NUMBER m33;
    Matrix3( NUMBER _m11, NUMBER _m12, NUMBER _m13, NUMBER _m21, NUMBER _m22, NUMBER _m23, NUMBER _m31, NUMBER _m32, NUMBER _m33 ) {
      m11 = _m11;
      m12 = _m12;
      m13 = _m13;
      m21 = _m21;
      m22 = _m22;
      m23 = _m23;
      m31 = _m31;
      m32 = _m32;
      m33 = _m33;
    }
    Matrix3 mul(Matrix3 other);
    Vector3 mul(Vector3 other);
    static Matrix3 unit();
    static Matrix3 rotate(NUMBER angle);
    static Matrix3 translate(Vector3 delta);
    static Matrix3 translate(NUMBER x, NUMBER y);
    static Matrix3 scale(Vector3 unit);
    static Matrix3 scale(NUMBER x, NUMBER y);
};
