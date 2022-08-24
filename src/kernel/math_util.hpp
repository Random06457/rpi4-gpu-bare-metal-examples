#pragma once

#include <cassert>
#include <cmath>
#include "types.hpp"

static constexpr f64 DEG_TO_RAD = M_PI / 180.0;
static constexpr f64 RAD_TO_DEG = 180.0 / M_PI;

template<typename T>
struct Vec3
{
    Vec3() = default;
    Vec3(T x, T y, T z) : x(x), y(y), z(z) {}
    union
    {
        struct
        {
            T x, y, z;
        };
        T xyz[3];
    };
} PACKED;

template<typename T>
struct Vec2
{
    union
    {
        struct
        {
            T x, y;
        };
        T xy[2];
    };
};

template<typename T>
struct Vec4
{
    Vec4() = default;
    Vec4(T x, T y, T z, T w) : x(x), y(y), z(z), w(w) {}

    union
    {
        struct
        {
            T x, y, z, w;
        };
        struct
        {
            T r, g, b, a;
        };
        T xyzw[4];
        T rgba[4];
    };
} PACKED;

using Vec2f = Vec2<f32>;
using Vec2u = Vec2<u32>;
using Vec3f = Vec3<f32>;
using Vec4f = Vec4<f32>;

template<typename T>
struct Box2
{
    Box2() = default;
    union
    {
        struct
        {
            T x, y, w, h;
        };
        struct
        {
            Vec2<T> pos;
            Vec2<T> scale;
        };
    };
};

using Box2f = Box2<f32>;

template<typename T>
struct Box3
{
    Box3() {}

    union
    {
        struct
        {
            T x, y, z, width, height, depth;
        };
        struct
        {
            Vec3<T> pos;
            Vec3<T> scale;
        };
    };
};

using Box3f = Box3<f32>;
using Viewport = Box3f;

template<size_t X, size_t Y>
struct Mat
{
    f32 m[Y][X];

    Mat() = default;

    Mat<Y, X> transpose()
    {
        Mat<Y, X> ret;

        for (size_t i = 0; i < X; i++)
            for (size_t j = 0; j < X; j++)
                ret.m[i][j] = m[j][i];

        return ret;
    }

    static Mat<X, Y> identity()
    {
        Mat<X, Y> ret;

        for (size_t y = 0; y < Y; y++)
            for (size_t x = 0; x < X; x++)
                ret.m[y][x] = (x == y) ? 1 : 0;

        return ret;
    }

    template<size_t X2, size_t Y2>
    Mat<X2, Y> operator*(const Mat<X2, Y2> m2)
    {
        static_assert(X2 == Y && X == Y2);

        Mat<X2, Y> ret;

        for (size_t i = 0; i < Y; i++)
        {
            for (size_t j = 0; j < X2; j++)
            {
                f32 acc = 0;
                for (size_t k = 0; k < X; k++)
                    acc += m[i][k] * m2.m[k][j];
                ret.m[i][j] = acc;
            }
        }

        return ret;
    }

    template<size_t X2, size_t Y2>
    Mat<X2, Y>& operator*=(const Mat<X2, Y2> m2)
    {
        *this = *this * m2;
        return *this;
    }

    Mat inverse()
    {
        if constexpr (X == 3 && Y == 3)
        {
            return inverse3();
        }
        if constexpr (X == 4 && Y == 4)
        {
            inverse4();
        }
        if constexpr ((X != 3 || Y != 3) && (X != 3 || Y != 3))
        {
            assert(0 && "Unsupported inverse");
        }
    }

    static Mat<4, 4> createFrustum(f32 left, f32 right, f32 bottom, f32 top,
                                   f32 near, f32 far);
    static Mat<4, 4> createPerspective(f32 fov, f32 aspect, f32 near, f32 far);

    static Mat<4, 4> rotateX(f32 angle);
    static Mat<4, 4> rotateY(f32 angle);
    static Mat<4, 4> rotateZ(f32 angle);
    static Mat<4, 4> translate(f32 x, f32 y, f32 z);
    static Mat<4, 4> scale(f32 x, f32 y, f32 z);

private:
    Mat<3, 3> inverse3();
    Mat<4, 4> inverse4();
} PACKED;

using Mat3 = Mat<3, 3>;
using Mat4 = Mat<4, 4>;
