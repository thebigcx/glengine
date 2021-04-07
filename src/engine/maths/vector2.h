#pragma once

#include <cassert>

#include "engine/maths/vector.h"

template<typename T>
class Vector<2, T>
{
public:
    Vector<2, T>()
        : x(0), y(0) {}

    Vector<2, T>(const T& v)
        : x(v), y(v) {}
    
    Vector<2, T>(const T& x_, const T& y_)
        : x(x_), y(y_) {}

    Vector<2, T>(const Vector<3, T>& v)
        : x(v.x), y(v.y) {}

    Vector<2, T>(const Vector<4, T>& v)
        : x(v.x), y(v.y) {}

    // Conversion
    template<typename U>
    Vector<2, T>(const Vector<2, U>& v)
        : x(static_cast<T>(v.x)), y(static_cast<T>(v.y)) {}

    template<typename A, typename B>
    Vector<2, T>(const A& x, const B& y)
        : x(static_cast<T>(x)), y(static_cast<T>(y)) {}

    T& operator[](uint32_t i)
    {
        assert(i > -1 && i < 2);

        switch (i)
        {
            case 0: return x;
            case 1: return y;
        }

        return x; // Never reaches
    }

    const T& operator[](uint32_t i) const
    {
        assert(i > -1 && i < 2);

        switch (i)
        {
            case 0: return x;
            case 1: return y;
        }

        return 0; // Never reaches
    }

    Vector<2, T>& operator=(const Vector<2, T>& v)
    {
        x = v.x;
        y = v.y;
        return *this;
    }

    template<typename U>
    Vector<2, T>& operator=(const Vector<2, U>& v)
    {
        x = static_cast<T>(v.x);
        y = static_cast<T>(v.y);
        return *this;
    }

    Vector<2, T>& operator+=(const Vector<2, T>& v)
    {
        return *this = *this + v;
    }

    Vector<2, T>& operator-=(const Vector<2, T>& v)
    {
        return *this = *this - v;
    }

    Vector<2, T>& operator*=(const Vector<2, T>& v)
    {
        return *this = *this * v;
    }

    Vector<2, T>& operator/=(const Vector<2, T>& v)
    {
        return *this = *this / v;
    }

    Vector<2, T>& operator+=(const T& v)
    {
        return *this = *this + v;
    }

    Vector<2, T>& operator-=(const T& v)
    {
        return *this = *this - v;
    }

    Vector<2, T>& operator*=(const T& v)
    {
        return *this = *this * v;
    }

    Vector<2, T>& operator/=(const T& v)
    {
        return *this = *this / v;
    }

    Vector<2, T>& operator++()
    {
        x++; y++;
        return *this;
    }

    Vector<2, T>& operator--()
    {
        x--; y--;
        return *this;
    }
    

    T x, y;
};

// Unary
template<typename T>
Vector<2, T> operator+(const Vector<2, T>& v)
{
    return v;
}

template<typename T>
Vector<2, T> operator-(const Vector<2, T>& v)
{
    return Vector<2, T>(-v.x, -v.y);
}

template<typename T>
Vector<2, T> operator+(const Vector<2, T>& a, const Vector<2, T>& b)
{
    return Vector<2, T>(a.x + b.x, a.y + b.y);
}

template<typename T>
Vector<2, T> operator-(const Vector<2, T>& a, const Vector<2, T>& b)
{
    return Vector<2, T>(a.x - b.x, a.y - b.y);
}

template<typename T>
Vector<2, T> operator*(const Vector<2, T>& a, const Vector<2, T>& b)
{
    return Vector<2, T>(a.x * b.x, a.y * b.y);
}

template<typename T>
Vector<2, T> operator/(const Vector<2, T>& a, const Vector<2, T>& b)
{
    return Vector<2, T>(a.x / b.x, a.y / b.y);
}

template<typename T>
Vector<2, T> operator+(const Vector<2, T>& a, const T& b)
{
    return Vector<2, T>(a.x + b, a.y + b);
}

template<typename T>
Vector<2, T> operator-(const Vector<2, T>& a, const T& b)
{
    return Vector<2, T>(a.x - b, a.y - b);
}

template<typename T>
Vector<2, T> operator*(const Vector<2, T>& a, const T& b)
{
    return Vector<2, T>(a.x * b, a.y * b);
}

template<typename T>
Vector<2, T> operator/(const Vector<2, T>& a, const T& b)
{
    return Vector<2, T>(a.x / b, a.y / b);
}

typedef Vector<2, float>        Vector2f;
typedef Vector<2, int>          Vector2i;
typedef Vector<2, long>         Vector2l;
typedef Vector<2, double>       Vector2d;
typedef Vector<2, unsigned int> Vector2u;