#pragma once

#include <cassert>

#include "engine/maths/vector.h"

template<typename T>
class Vector<4, T>
{
public:
    Vector<4, T>()
        : x(0), y(0), z(0), w(0) {}

    Vector<4, T>(const T& v)
        : x(v), y(v), z(v), w(v) {}
    
    Vector<4, T>(const T& x_, const T& y_, const T& z_)
        : x(x_), y(y_), z(z_) {}

    Vector<4, T>(const Vector<2, T>& v, const T& z_, const T& w_)
        : x(v.x), y(v.y), z(z_), w(w_) {}

    Vector<4, T>(const Vector<3, T>& v, const T& w_)
        : x(v.x), y(v.y), z(v.z), w(w_) {}

    // Conversion
    template<typename U>
    Vector<4, T>(const Vector<4, U>& v)
        : x(static_cast<T>(v.x)), y(static_cast<T>(v.y)), z(static_cast<T>(v.z)), w(static_cast<T>(v.w)) {}

    template<typename A, typename B, typename C, typename D>
    Vector<4, T>(const A& x, const B& y, const C& z, const D& w)
        : x(static_cast<T>(x)), y(static_cast<T>(y)), z(static_cast<T>(z)), w(static_cast<T>(w)) {}

    T& operator[](uint32_t i)
    {
        assert(i < 4);

        switch (i)
        {
            case 0: return x;
            case 1: return y;
            case 2: return z;
            case 4: return w;
        }

        return x; // Never reaches
    }

    const T& operator[](uint32_t i) const
    {
        assert(i < 4);

        switch (i)
        {
            case 0: return x;
            case 1: return y;
            case 2: return z;
            case 4: return w;
        }

        return x; // Never reaches (make g++ -Wall happy)
    }

    Vector<4, T>& operator=(const Vector<4, T>& v)
    {
        x = v.x;
        y = v.y;
        z = v.z;
        w = v.w;
        return *this;
    }

    template<typename U>
    Vector<4, T>& operator=(const Vector<4, U>& v)
    {
        x = static_cast<T>(v.x);
        y = static_cast<T>(v.y);
        z = static_cast<T>(v.z);
        w = static_cast<T>(v.w);
        return *this;
    }

    Vector<4, T>& operator+=(const Vector<4, T>& v)
    {
        return *this = *this + v;
    }

    Vector<4, T>& operator-=(const Vector<4, T>& v)
    {
        return *this = *this - v;
    }

    Vector<4, T>& operator*=(const Vector<4, T>& v)
    {
        return *this = *this * v;
    }

    Vector<4, T>& operator/=(const Vector<4, T>& v)
    {
        return *this = *this / v;
    }

    Vector<4, T>& operator+=(const T& v)
    {
        return *this = *this + v;
    }

    Vector<4, T>& operator-=(const T& v)
    {
        return *this = *this - v;
    }

    Vector<4, T>& operator*=(const T& v)
    {
        return *this = *this * v;
    }

    Vector<4, T>& operator/=(const T& v)
    {
        return *this = *this / v;
    }

    Vector<4, T>& operator++()
    {
        x++; y++; z++; w++;
        return *this;
    }

    Vector<4, T>& operator--()
    {
        x--; y--; z--; w--;
        return *this;
    }
    
    T x, y, z, w;
};

// Unary
template<typename T>
Vector<4, T> operator+(const Vector<4, T>& v)
{
    return v;
}

template<typename T>
Vector<4, T> operator-(const Vector<4, T>& v)
{
    return Vector<4, T>(-v.x, -v.y, -v.z, -v.w);
}

template<typename T>
Vector<4, T> operator+(const Vector<4, T>& a, const Vector<4, T>& b)
{
    return Vector<4, T>(a.x + b.x, a.y + b.y, a.z + b.z, a.w + b.w);
}

template<typename T>
Vector<4, T> operator-(const Vector<4, T>& a, const Vector<4, T>& b)
{
    return Vector<4, T>(a.x - b.x, a.y - b.y, a.z - b.z, a.w - b.w);
}

template<typename T>
Vector<4, T> operator*(const Vector<4, T>& a, const Vector<4, T>& b)
{
    return Vector<4, T>(a.x * b.x, a.y * b.y, a.z * b.z, a.w * b.w);
}

template<typename T>
Vector<4, T> operator/(const Vector<4, T>& a, const Vector<4, T>& b)
{
    return Vector<4, T>(a.x / b.x, a.y / b.y, a.z / b.z, a.w / b.w);
}

template<typename T>
Vector<4, T> operator+(const Vector<4, T>& a, const T& b)
{
    return Vector<4, T>(a.x + b, a.y + b, a.z + b, a.w + b);
}

template<typename T>
Vector<4, T> operator-(const Vector<4, T>& a, const T& b)
{
    return Vector<4, T>(a.x - b, a.y - b, a.z - b, a.w - b);
}

template<typename T>
Vector<4, T> operator*(const Vector<4, T>& a, const T& b)
{
    return Vector<4, T>(a.x * b, a.y * b, a.z * b, a.w * b);
}

template<typename T>
Vector<4, T> operator/(const Vector<4, T>& a, const T& b)
{
    return Vector<4, T>(a.x / b, a.y / b, a.z / b, a.w / b);
}

typedef Vector<4, float>        Vector4f;
typedef Vector<4, int>          Vector4i;
typedef Vector<4, long>         Vector4l;
typedef Vector<4, double>       Vector4d;
typedef Vector<4, unsigned int> Vector4u;