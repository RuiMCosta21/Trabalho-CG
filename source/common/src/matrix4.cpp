#include "matrix4.hpp"
#include <cmath>
#include <algorithm>
#include <iostream>
#include <iomanip> 


Matrix4::Matrix4() {
    identity();
}

void Matrix4::identity() {
    std::fill(m, m + 16, 0.0f);
    m[0] = m[5] = m[10] = m[15] = 1.0f;
}

Matrix4 Matrix4::multiply(const Matrix4& other) const {
    Matrix4 res;
    for (int col = 0; col < 4; ++col) {
        for (int row = 0; row < 4; ++row) {
            float sum = 0.0f;
            for (int i = 0; i < 4; ++i) {
                sum += m[i * 4 + row] * other.m[col * 4 + i];
            }
            res.m[col * 4 + row] = sum;
        }
    }
    return res;
}

Matrix4 Matrix4::translation(float x, float y, float z) {
    Matrix4 res;
    res.m[12] = x; res.m[13] = y; res.m[14] = z;
    return res;
}

Matrix4 Matrix4::scale(float x, float y, float z) {
    Matrix4 res;
    res.m[0] = x; res.m[5] = y; res.m[10] = z;
    return res;
}

Matrix4 Matrix4::rotation(float angle, float x, float y, float z) {
    Matrix4 res;
    float rad = angle * M_PI / 180.0f;
    float c = cosf(rad), s = sinf(rad), t = 1.0f - c;
    float len = sqrtf(x*x + y*y + z*z);
    if (len > 0) { x/=len; y/=len; z/=len; }

    res.m[0] = t*x*x + c;   res.m[4] = t*x*y - s*z; res.m[8] = t*x*z + s*y;
    res.m[1] = t*x*y + s*z; res.m[5] = t*y*y + c;   res.m[9] = t*y*z - s*x;
    res.m[2] = t*x*z - s*y; res.m[6] = t*y*z + s*x; res.m[10] = t*z*z + c;
    return res;
}

void Matrix4::print() const {
    std::cout << "Matrix4 Output:" << std::endl;
    std::cout << "-------------------------------" << std::endl;
    
    for (int row = 0; row < 4; ++row) {
        std::cout << "| ";
        for (int col = 0; col < 4; ++col) {
            // Indexing formula for Column-Major: [column * 4 + row]
            float value = m[col * 4 + row];
            
            // Format the output to 3 decimal places with padding
            std::cout << std::setw(8) << std::fixed << std::setprecision(3) << value << " ";
        }
        std::cout << " |" << std::endl;
    }
    std::cout << "-------------------------------" << std::endl;
}