#pragma once

#include <cassert>
#include <cmath>

#include "engine/maths/vector.h"

template<typename T>
class Vector<3, T>
{
public:
    Vector<3, T>()
        : x(0), y(0), z(0) {}

    Vector<3, T>(const T& v)
        : x(v), y(v), z(v) {}
    
    Vector<3, T>(const T& x_, const T& y_, const T& z_)
        : x(x_), y(y_), z(z_) {}

    Vector<3, T>(const Vector<2, T>& v, const T& z_)
        : x(v.x), y(v.y), z(z_) {}

    Vector<3, T>(const Vector<4, T>& v)
        : x(v.x), y(v.y), z(v.z) {}

    // Conversion
    template<typename U>
    Vector<3, T>(const Vector<3, U>& v)
        : x(static_cast<T>(v.x)), y(static_cast<T>(v.y)), z(static_cast<T>(v.z)) {}

    template<typename A, typename B, typename C>
    Vector<3, T>(const A& x, const B& y, const C& z)
        : x(static_cast<T>(x)), y(static_cast<T>(y)), z(static_cast<T>(z)) {}

    T& operator[](uint32_t i)
    {
        assert(i < 3);

        switch (i)
        {
            case 0: return x;
            case 1: return y;
            case 2: return z;
        }

        return x; // Never reaches
    }

    const T& operator[](uint32_t i) const
    {
        assert(i < 3);

        switch (i)
        {
            case 0: return x;
            case 1: return y;
            case 2: return z;
        }

        return 0; // Never reaches
    }

    Vector<3, T>& operator=(const Vector<3, T>& v)
    {
        x = v.x;
        y = v.y;
        z = v.z;
        return *this;
    }

    template<typename U>
    Vector<3, T>& operator=(const Vector<3, U>& v)
    {
        x = static_cast<T>(v.x);
        y = static_cast<T>(v.y);
        z = static_cast<T>(v.z);
        return *this;
    }

    Vector<3, T>& operator+=(const Vector<3, T>& v)
    {
        return *this = *this + v;
    }

    Vector<3, T>& operator-=(const Vector<3, T>& v)
    {
        return *this = *this - v;
    }

    Vector<3, T>& operator*=(const Vector<3, T>& v)
    {
        return *this = *this * v;
    }

    Vector<3, T>& operator/=(const Vector<3, T>& v)
    {
        return *this = *this / v;
    }

    Vector<3, T>& operator+=(const T& v)
    {
        return *this = *this + v;
    }

    Vector<3, T>& operator-=(const T& v)
    {
        return *this = *this - v;
    }

    Vector<3, T>& operator*=(const T& v)
    {
        return *this = *this * v;
    }

    Vector<3, T>& operator/=(const T& v)
    {
        return *this = *this / v;
    }

    Vector<3, T>& operator++()
    {
        x++; y++; z++;
        return *this;
    }

    Vector<3, T>& operator--()
    {
        x--; y--; z--;
        return *this;
    }

    bool operator==(const Vector<3 , T>& v)
    {
        return x == v.x && y == v.y && z != v.z;
    }

    bool operator!=(const Vector<3 , T>& v)
    {
        return x != v.x || y != v.y || z != v.z;
    }

    static float mag(const Vector<3, T>& v);
    static Vector<3, T> normalize(const Vector<3, T>& v);
    static Vector<3, T> cross(const Vector<3, T>& a, const Vector<3, T>& b);
    static T dot(const Vector<3, T>& a, const Vector<3, T>& b);
    
    T x, y, z;
};

// Unary
template<typename T>
Vector<3, T> operator+(const Vector<3, T>& v)
{
    return v;
}

template<typename T>
Vector<3, T> operator-(const Vector<3, T>& v)
{
    return Vector<3, T>(-v.x, -v.y, -v.z);
}

template<typename T>
Vector<3, T> operator+(const Vector<3, T>& a, const Vector<3, T>& b)
{
    return Vector<3, T>(a.x + b.x, a.y + b.y, a.z + b.z);
}

template<typename T>
Vector<3, T> operator-(const Vector<3, T>& a, const Vector<3, T>& b)
{
    return Vector<3, T>(a.x - b.x, a.y - b.y, a.z - b.z);
}

template<typename T>
Vector<3, T> operator*(const Vector<3, T>& a, const Vector<3, T>& b)
{
    return Vector<3, T>(a.x * b.x, a.y * b.y, a.z * b.z);
}

template<typename T>
Vector<3, T> operator/(const Vector<3, T>& a, const Vector<3, T>& b)
{
    return Vector<3, T>(a.x / b.x, a.y / b.y, a.z / b.z);
}

template<typename T>
Vector<3, T> operator+(const Vector<3, T>& a, const T& b)
{
    return Vector<3, T>(a.x + b, a.y + b, a.z + b);
}

template<typename T>
Vector<3, T> operator-(const Vector<3, T>& a, const T& b)
{
    return Vector<3, T>(a.x - b, a.y - b, a.z - b);
}

template<typename T>
Vector<3, T> operator*(const Vector<3, T>& a, const T& b)
{
    return Vector<3, T>(a.x * b, a.y * b, a.z * b);
}

template<typename T>
Vector<3, T> operator/(const Vector<3, T>& a, const T& b)
{
    return Vector<3, T>(a.x / b, a.y / b, a.z / b);
}

template<typename T>
float Vector<3, T>::mag(const Vector<3, T>& v)
{
    return std::sqrt(v.x * v.x + v.y * v.y + v.z * v.z);
}

template<typename T>
Vector<3, T> Vector<3, T>::normalize(const Vector<3, T>& v)
{
    float m = 1.f / Vector<3, T>::mag(v);
    return Vector<3, T>(v.x * m, v.y * m, v.z * m);
}

template<typename T>
Vector<3, T> Vector<3, T>::cross(const Vector<3, T>& a, const Vector<3, T>& b)
{
    return Vector<3, T>(a.y * b.z - a.z * b.y, a.z * b.x - a.x * b.z, a.x * b.y - a.y * b.x);
}

template<typename T>
T Vector<3, T>::dot(const Vector<3, T>& a, const Vector<3, T>& b)
{
    return a.x * b.x + a.y * b.y + a.z * b.z;
}

typedef Vector<3, float>        Vector3f;
typedef Vector<3, int>          Vector3i;
typedef Vector<3, long>         Vector3l;
typedef Vector<3, double>       Vector3d;
typedef Vector<3, unsigned int> Vector3u;