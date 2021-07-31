class Vector3 {
  public:
    int x;
    int y;
    int isPoint;
    Vector3(int _x, int _y, int _isPoint) {
      x = _x;
      y = _y;
      isPoint = _isPoint;
    }
    static Vector3 vector(int x, int y);
    static Vector3 point(int x, int y);
};

class Matrix3 {
  public:
    int m11;
    int m12;
    int m13;
    int m21;
    int m22;
    int m23;
    int m31;
    int m32;
    int m33;
    Matrix3( int _m11, int _m12, int _m13, int _m21, int _m22, int _m23, int _m31, int _m32, int _m33 ) {
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
    static Matrix3 rotate(int angle);
    static Matrix3 translate(Vector3 delta);
    static Matrix3 scale(Vector3 unit);
};
