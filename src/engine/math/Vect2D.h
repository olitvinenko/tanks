#pragma once

#include <algorithm>
#include <cmath>
#include <cassert>
#include <cstdlib>

#include "Constants.h"
#include "common/Types.h"

template <typename T>
class Vec2D
{
public:
    T x;
    T y;
    
    Vec2D()
        : x(0.0f)
        , y(0.0f)
    {
    }
    
    Vec2D(float _x, float _y)
        : x(_x)
        , y(_y)
    {
    }

    float operator[](unsigned int i) const
    {
        assert(i < 2);
        return (&x)[i];
    }

    Vec2D<T>& operator-=(const Vec2D<T>& v)
    {
        x -= v.x;
        y -= v.y;
        return *this;
    }

    Vec2D<T>& operator+=(const Vec2D<T>& v)
    {
        x += v.x;
        y += v.y;
        return *this;
    }

    Vec2D<T> operator + (const Vec2D<T>& v) const
    {
        return Vec2D{ x + v.x, y + v.y };
    }

    Vec2D<T> operator - (const Vec2D<T>& v) const
    {
        return Vec2D{ x - v.x, y - v.y };
    }

    Vec2D<T> operator - () const
    {
        return Vec2D<T>{ -x, -y };
    }

    Vec2D<T> operator * (T a) const
    {
        return Vec2D<T>{ x * a, y * a };
    }

    Vec2D<T> operator * (const Vec2D<T>& v) const
    {
        return Vec2D<T>{ x * v.x, y * v.y };
    }

    Vec2D operator / (T a) const
    {
        return Vec2D{ x / a, y / a };
    }

    Vec2D<T> operator / (const Vec2D<T>& v) const
    {
        return Vec2D<T>{ x / v.x, y / v.y };
    }

    const Vec2D<T>& operator *= (T a)
    {
        x *= a;
        y *= a;
        return *this;
    }

    const Vec2D<T>& operator /= (T a)
    {
        x /= a;
        y /= a;
        return *this;
    }
    
    Vec2D<T>& operator = (const Vec2D<T>& v)
    {
        x = v.x;
        y = v.y;
        return *this;
    }

    friend Vec2D<T> operator * (float a, const Vec2D<T>& v)
    {
        return Vec2D<T>{ v.x * a, v.y * a };
    }

    bool operator ==(const Vec2D<T>& v) const
    {
        return v.x == x && v.y == y;
    }

    bool operator !=(const Vec2D<T>& v) const
    {
        return v.x != x || v.y != y;
    }

    T sqr() const
    {
        return x*x + y*y;
    }

    T len() const
    {
        return sqrt(x*x + y*y);
    }

    bool IsZero() const
    {
        return x == 0 && y == 0;
    }

    T Angle() const // angle to the X axis
    {
        T a = atan2(y, x);
        return (a < 0) ? (a + PI2) : a;
    }

    Vec2D<T>& Normalize()
    {
        T len = sqrt(x*x + y*y);
        if( len < 1e-7 )
        {
            x = 0;
            y = 0;
        }
        else
        {
            x /= len;
            y /= len;
        }
        return *this;
    }
    
    T Distance(const Vec2D<T>& v)const
    {
        float ySeparation = v.y - y;
        float xSeparation = v.x - x;

        return sqrt(ySeparation * ySeparation + xSeparation * xSeparation);
    }

    Vec2D<T> Norm() const
    {
        Vec2D<T> result = *this;
        return result.Normalize();
    }
};

template<typename T>
inline T Vec2DDistance(const Vec2D<T>& v1, const Vec2D<T>& v2)
{
    return v1.Distance(v2);
}

template<typename T>
inline Vec2D<T> Vec2dFloor(const Vec2D<T>& vec)
{
    return{ std::floor(vec.x), std::floor(vec.y) };
}

template<typename T>
inline Vec2D<T> Vec2dFloor(T x, T y)
{
    return{ std::floor(x), std::floor(y) };
}

template<typename T>
inline Vec2D<T> Vec2dDirection(T angle)
{
    return{ cosf(angle), sinf(angle) };
}

template<typename T>
inline Vec2D<T> Vec2dAddDirection(const Vec2D<T>& a, const Vec2D<T>& b)
{
    assert(std::abs(a.sqr() - 1) < 1e-5);
    assert(std::abs(b.sqr() - 1) < 1e-5);
    return{ a.x*b.x - a.y*b.y, a.y*b.x + a.x*b.y };
}

template<typename T>
inline Vec2D<T> Vec2dSubDirection(const Vec2D<T>& a, const Vec2D<T>& b)
{
    assert(std::abs(a.sqr() - 1) < 1e-5);
    assert(std::abs(b.sqr() - 1) < 1e-5);
    return{ a.x*b.x + a.y*b.y, a.y*b.x - a.x*b.y };
}

template<typename T>
inline T Vec2dCross(const Vec2D<T>& a, const Vec2D<T>& b)
{
    return a.x*b.y - a.y*b.x;
}

template<typename T>
inline T Vec2dDot(const Vec2D<T>& a, const Vec2D<T>& b)
{
    return a.x*b.x + a.y*b.y;
}

template<typename T>
inline Vec2D<T> Vec2dMulX(const Vec2D<T>& v, T m)
{
    return Vec2D<T>{ v.x * m, v.y };
}

template<typename T>
inline Vec2D<T> Vec2dMulY(const Vec2D<T>& v, float m)
{
    return Vec2D<T>{ v.x, v.y * m };
}

// generates a pseudo random number in range [0, max)
inline float frand(float max)
{
    return (float) rand() / RAND_MAX * max;
}

template<typename T>
// generates a pseudo random vector of the specified length
inline Vec2D<T> vrand(T len)
{
    return Vec2dDirection<T>(frand(PI2)) * len;
}

using Vec2F = Vec2D<float32>;
