#include "math_util.hpp"
#include <cmath>

template<>
Mat3 Mat3::inverse3()
{
    // https://stackoverflow.com/questions/983999/simple-3x3-matrix-inverse-code-c

    Mat<3, 3> ret;

    f32 det = m[0][0] * (m[1][1] * m[2][2] - m[2][1] * m[1][2]) -
              m[0][1] * (m[1][0] * m[2][2] - m[1][2] * m[2][0]) +
              m[0][2] * (m[1][0] * m[2][1] - m[1][1] * m[2][0]);

    assert(det != 0 && "0 determinant");

    det = 1 / det;

    ret.m[0][0] = (m[1][1] * m[2][2] - m[2][1] * m[1][2]) * det;
    ret.m[0][1] = (m[0][2] * m[2][1] - m[0][1] * m[2][2]) * det;
    ret.m[0][2] = (m[0][1] * m[1][2] - m[0][2] * m[1][1]) * det;
    ret.m[1][0] = (m[1][2] * m[2][0] - m[1][0] * m[2][2]) * det;
    ret.m[1][1] = (m[0][0] * m[2][2] - m[0][2] * m[2][0]) * det;
    ret.m[1][2] = (m[1][0] * m[0][2] - m[0][0] * m[1][2]) * det;
    ret.m[2][0] = (m[1][0] * m[2][1] - m[2][0] * m[1][1]) * det;
    ret.m[2][1] = (m[2][0] * m[0][1] - m[0][0] * m[2][1]) * det;
    ret.m[2][2] = (m[0][0] * m[1][1] - m[1][0] * m[0][1]) * det;

    return ret;
}

template<>
Mat4 Mat4::inverse4()
{
    // https://stackoverflow.com/questions/1148309/inverting-a-4x4-matrix

    Mat<4, 4> ret;

    f32 a2323 = m[2][2] * m[3][3] - m[2][3] * m[3][2];
    f32 a1323 = m[2][1] * m[3][3] - m[2][3] * m[3][1];
    f32 a1223 = m[2][1] * m[3][2] - m[2][2] * m[3][1];
    f32 a0323 = m[2][0] * m[3][3] - m[2][3] * m[3][0];
    f32 a0223 = m[2][0] * m[3][2] - m[2][2] * m[3][0];
    f32 a0123 = m[2][0] * m[3][1] - m[2][1] * m[3][0];
    f32 a2313 = m[1][2] * m[3][3] - m[1][3] * m[3][2];
    f32 a1313 = m[1][1] * m[3][3] - m[1][3] * m[3][1];
    f32 a1213 = m[1][1] * m[3][2] - m[1][2] * m[3][1];
    f32 a2312 = m[1][2] * m[2][3] - m[1][3] * m[2][2];
    f32 a1312 = m[1][1] * m[2][3] - m[1][3] * m[2][1];
    f32 a1212 = m[1][1] * m[2][2] - m[1][2] * m[2][1];
    f32 a0313 = m[1][0] * m[3][3] - m[1][3] * m[3][0];
    f32 a0213 = m[1][0] * m[3][2] - m[1][2] * m[3][0];
    f32 a0312 = m[1][0] * m[2][3] - m[1][3] * m[2][0];
    f32 a0212 = m[1][0] * m[2][2] - m[1][2] * m[2][0];
    f32 a0113 = m[1][0] * m[3][1] - m[1][1] * m[3][0];
    f32 a0112 = m[1][0] * m[2][1] - m[1][1] * m[2][0];

    f32 det = m[0][0] * (m[1][1] * a2323 - m[1][2] * a1323 + m[1][3] * a1223) -
              m[0][1] * (m[1][0] * a2323 - m[1][2] * a0323 + m[1][3] * a0223) +
              m[0][2] * (m[1][0] * a1323 - m[1][1] * a0323 + m[1][3] * a0123) -
              m[0][3] * (m[1][0] * a1223 - m[1][1] * a0223 + m[1][2] * a0123);

    assert(det != 0 && "0 determinant");

    det = 1 / det;

    ret.m[0][0] = det * (m[1][1] * a2323 - m[1][2] * a1323 + m[1][3] * a1223);
    ret.m[0][1] = det * -(m[0][1] * a2323 - m[0][2] * a1323 + m[0][3] * a1223);
    ret.m[0][2] = det * (m[0][1] * a2313 - m[0][2] * a1313 + m[0][3] * a1213);
    ret.m[0][3] = det * -(m[0][1] * a2312 - m[0][2] * a1312 + m[0][3] * a1212);
    ret.m[1][0] = det * -(m[1][0] * a2323 - m[1][2] * a0323 + m[1][3] * a0223);
    ret.m[1][1] = det * (m[0][0] * a2323 - m[0][2] * a0323 + m[0][3] * a0223);
    ret.m[1][2] = det * -(m[0][0] * a2313 - m[0][2] * a0313 + m[0][3] * a0213);
    ret.m[1][3] = det * (m[0][0] * a2312 - m[0][2] * a0312 + m[0][3] * a0212);
    ret.m[2][0] = det * (m[1][0] * a1323 - m[1][1] * a0323 + m[1][3] * a0123);
    ret.m[2][1] = det * -(m[0][0] * a1323 - m[0][1] * a0323 + m[0][3] * a0123);
    ret.m[2][2] = det * (m[0][0] * a1313 - m[0][1] * a0313 + m[0][3] * a0113);
    ret.m[2][3] = det * -(m[0][0] * a1312 - m[0][1] * a0312 + m[0][3] * a0112);
    ret.m[3][0] = det * -(m[1][0] * a1223 - m[1][1] * a0223 + m[1][2] * a0123);
    ret.m[3][1] = det * (m[0][0] * a1223 - m[0][1] * a0223 + m[0][2] * a0123);
    ret.m[3][2] = det * -(m[0][0] * a1213 - m[0][1] * a0213 + m[0][2] * a0113);
    ret.m[3][3] = det * (m[0][0] * a1212 - m[0][1] * a0212 + m[0][2] * a0112);

    return ret;
}

template<>
Mat4 Mat4::createFrustum(f32 left, f32 right, f32 bottom, f32 top, f32 near,
                         f32 far)
{
    Mat4 ret;

    ret.m[0][0] = 2 * near / (right - left);
    ret.m[0][1] = 0;
    ret.m[0][2] = 0;
    ret.m[0][3] = -near * (right + left) / (right - left);

    ret.m[1][0] = 0;
    ret.m[1][1] = 2 * near / (top - bottom);
    ret.m[1][2] = 0;
    ret.m[1][3] = -near * (top + bottom) / (top - bottom);

    ret.m[2][0] = 0;
    ret.m[2][1] = 0;
    ret.m[2][2] = -(far + near) / (far - near);
    ret.m[2][3] = 2 * far * near / (near - far);

    ret.m[3][0] = 0;
    ret.m[3][1] = 0;
    ret.m[3][2] = -1;
    ret.m[3][3] = 0;

    return ret;
}

template<>
Mat4 Mat4::createPerspective(f32 fov, f32 aspect, f32 near, f32 far)
{
    f32 top = near * tan(fov * 0.5 * M_PI / 180);
    f32 bottom = -top;
    f32 right = top * aspect;
    f32 left = -right;

    return createFrustum(left, right, bottom, top, near, far);
}

template<>
Mat4 Mat4::rotateX(f32 angle)
{
    Mat4 ret = Mat4::identity();

    ret.m[1][1] = cos(angle);
    ret.m[1][2] = -sin(angle);
    ret.m[2][1] = sin(angle);
    ret.m[2][2] = cos(angle);

    return ret;
}
template<>
Mat4 Mat4::rotateY(f32 angle)
{
    Mat4 ret = Mat4::identity();

    ret.m[0][0] = cos(angle);
    ret.m[0][2] = sin(angle);
    ret.m[2][0] = -sin(angle);
    ret.m[2][2] = cos(angle);

    return ret;
}
template<>
Mat4 Mat4::rotateZ(f32 angle)
{
    Mat4 ret = Mat4::identity();

    ret.m[0][0] = cos(angle);
    ret.m[0][1] = -sin(angle);
    ret.m[1][0] = sin(angle);
    ret.m[1][1] = cos(angle);

    return ret;
}

template<>
Mat<4, 4> Mat4::translate(f32 x, f32 y, f32 z)
{
    Mat4 ret = Mat4::identity();

    ret.m[3][0] = x;
    ret.m[3][1] = y;
    ret.m[3][2] = z;

    return ret;
}
template<>
Mat<4, 4> Mat4::scale(f32 x, f32 y, f32 z)
{
    Mat4 ret = Mat4::identity();

    ret.m[0][0] = x;
    ret.m[1][1] = y;
    ret.m[2][2] = z;

    return ret;
}
