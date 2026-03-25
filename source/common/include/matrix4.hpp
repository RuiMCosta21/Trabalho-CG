#ifndef MATRIX4_H
#define MATRIX4_H

struct Matrix4 {
    float m[16];

    Matrix4();
    void identity();
    Matrix4 multiply(const Matrix4& other) const;
    static Matrix4 translation(float x, float y, float z);
    static Matrix4 scale(float x, float y, float z);
    static Matrix4 rotation(float angleDegrees, float x, float y, float z);
    void print() const;
};

#endif