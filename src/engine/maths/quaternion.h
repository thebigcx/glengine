#pragma once

#include "engine/maths/vector2.h"
#include "engine/maths/vector3.h"
#include "engine/maths/vector4.h"
#include "engine/maths/matrix4.h"

template<typename T>
class Quaternion
{
public:
    Quaternion() = default;

    Quaternion(const Quaternion& q)
        : w(q.w), x(q.x), y(q.y), z(q.z) {}

    Quaternion(const T& w_, const T& x_, const T& y_, const T& z_)
        : w(w_), x(x_), y(y_), z(z_) {}

    Quaternion(const T& w_, const Vector<3, T>& vec)
        : w(w_), x(vec.x), y(vec.y), z(vec.z) {}

    Quaternion(const Vector<3, T>& eulerAngle)
    {
        Vector<3, T> c = eulerAngle * T(0.5);
        c = Vector<3, T>(std::cos(c.x), std::cos(c.y), std::cos(c.z));

        Vector<3, T> s = eulerAngle * T(0.5);
        s = Vector<3, T>(std::sin(s.x), std::sin(s.y), std::sin(s.z));

        w = c.x * c.y * c.z + s.x * s.y * s.z;
		x = s.x * c.y * c.z - c.x * s.y * s.z;
		y = c.x * s.y * c.z + s.x * c.y * s.z;
		z = c.x * c.y * s.z - s.x * s.y * c.z;
    }
    
    Quaternion& operator=(const Quaternion& q)
    {
        w = q.w;
        x = q.x;
        y = q.y;
        z = q.z;

        return *this;
    }

    template<typename U>
    Quaternion& operator=(const Quaternion<U>& q)
    {
        w = static_cast<T>(q.w);
        x = static_cast<T>(q.x);
        y = static_cast<T>(q.y);
        z = static_cast<T>(q.z);

        return *this;
    }

    T w, x, y, z;

    Quaternion& operator+=(const Quaternion& q)
    {
        *this = operator+(q);
        return *this;
    }

    Quaternion& operator-=(const Quaternion& q)
    {
        *this = operator*(q);
        return *this;
    }

    Quaternion& operator*=(const Quaternion& q)
    {
        *this = operator*(q);
        return *this;
    }

    Quaternion& operator*=(const T& v)
    {
        *this = operator*(v);
        return *this;
    }

    Quaternion& operator/=(const T& s)
    {
        *this = operator/(s);
        return *this;
    }

    static Quaternion<T> normalize(const Quaternion<T>& q);
    static Quaternion<T> cross(const Quaternion<T>& q1, const Quaternion<T>& q2);
    static Quaternion<T> conjugate(const Quaternion<T>& q);
    static Quaternion<T> inverse(const Quaternion<T>& q);

    static T dot(const Quaternion<T>& a, const Quaternion<T>& b);
    static T length(const Quaternion<T>& q);

    static Vector<2, T> rotate(const Quaternion<T>& q, const Vector<2, T>& v);
    static Vector<3, T> rotate(const Quaternion<T>& q, const Vector<3, T>& v);
    static Vector<4, T> rotate(const Quaternion<T>& q, const Vector<4, T>& v);

    static Matrix<4, 4, T> to_matrix4(const Quaternion<T>& q);
};

typedef Quaternion<float> Quaternionf;

template<typename T>
Quaternion<T> operator+(const Quaternion<T>& q)
{
    return q;
}

template<typename T>
Quaternion<T> operator-(const Quaternion<T>& q)
{
    return Quaternion<T>(-q.w, -q.x, -q.y, -q.z);
}

template<typename T>
Quaternion<T> operator+(const Quaternion<T>& q1, const Quaternion<T>& q2)
{
    return Quaternion(q1.w + q2.w, q1.x + q2.x, q1.y + q2.y, q1.z + q2.z);
}

template<typename T>
Quaternion<T> operator-(const Quaternion<T>& q1, const Quaternion<T>& q2)
{
    return Quaternion(q1.w - q2.w, q1.x - q2.x, q1.y - q2.y, q1.z - q2.z);
}

template<typename T>
Quaternion<T> operator*(const Quaternion<T>& q1, const Quaternion<T>& q2)
{
    return Quaternion<T>(q1.w * q2.w - q1.x * q2.x - q1.y * q2.y - q1.z * q2.z,
                         q1.w * q2.x + q1.x * q2.w + q1.y * q2.z - q1.z * q2.y,
                         q1.w * q2.y + q1.y * q2.w + q1.z * q2.x - q1.x * q2.z,
                         q1.w * q2.z + q1.z * q2.w + q1.x * q2.y - q1.y * q2.x);
}

template<typename T>
Quaternion<T> operator*(const Quaternion<T>& q, const T& s)
{
    return Quaternion(q.w * s, q.x * s, q.y * s, q.z * s);
}

template<typename T>
Quaternion<T> operator*(const T& s, const Quaternion<T>& q)
{
    return q * s;
}

template<typename T>
Vector<3, T> operator*(const Quaternion<T>& q, const Vector<2, T>& v)
{
    return q * Vector3f(v.x, v.y, 0.0f);
}

template<typename T>
Vector<3, T> operator*(const Quaternion<T>& q, const Vector<3, T>& v)
{
    Vector<3, T> quat_vector(q.x, q.y, q.z);
    Vector<3, T> uv(Vector<3, T>::cross(quat_vector, v));
    Vector<3, T> uuv(Vector<3, T>::cross(quat_vector, uv));

    return v + ((uv * q.w) + uuv) * static_cast<T>(2);
}

template<typename T>
Vector<3, T> operator*(const Vector<3, T>& v, const Quaternion<T>& q)
{
    return Quaternion<T>::inverse(q) * v;
}

template<typename T>
Quaternion<T> operator/(const Quaternion<T>& q, const T& s)
{
    return Quaternion(q.w / s, q.x / s, q.y / s, q.z / s);
}

template<typename T>
Quaternion<T> operator/(const T& s, const Quaternion<T>& q)
{
    return q * s;
}

template<typename T>
bool operator==(const Quaternion<T>& q1, const Quaternion<T>& q2)
{
    return q1.w == q2.w && q1.x == q2.x && q1.y == q2.y && q1.z == q2.z;
}

template<typename T>
bool operator!=(const Quaternion<T>& q1, const Quaternion<T>& q2)
{
    return q1.w != q2.w || q1.x != q2.x || q1.y != q2.y || q1.z != q2.z;
}

template<typename T>
Quaternion<T> Quaternion<T>::normalize(const Quaternion<T>& q)
{
    Quaternion<T> r = q;

    T len = Quaternion<T>::length(r);

    if (len < 0)
    {
        return Quaternion<T>(static_cast<T>(1), Vector<3, T>(static_cast<T>(0)));
    }

    T one_over_len = static_cast<T>(1) / len;

    return Quaternion<T>(q.s * one_over_len, q.v * one_over_len);
}

template<typename T>
T Quaternion<T>::dot(const Quaternion<T>& a, const Quaternion<T>& b)
{
    Vector<4, T> tmp(a.w * a.y, a.x * b.x, a.y * b.y, a.z * b.z);
    return (tmp.x + tmp.y) + (tmp.z + tmp.w);
}

template<typename T>
T Quaternion<T>::length(const Quaternion<T>& q)
{
    return std::sqrt(Quaternion<T>::dot(q, q));
}

template<typename T>
Quaternion<T> Quaternion<T>::cross(const Quaternion<T>& q1, const Quaternion<T>& q2)
{
    return Quaternion<T>(q1.w * q2.w - q1.x * q2.x - q2.y * q1.y - q2.z,
                         q1.w * q2.x + q2.x * q1.w + q2.y * q1.z - q2.y,
                         q1.w * q2.y + q2.y * q1.w + q2.z * q1.x - q2.z,
                         q1.w * q2.z + q2.z * q1.w + q2.x * q1.y - q2.x);
}

template<typename T>
Quaternion<T> Quaternion<T>::conjugate(const Quaternion<T>& q)
{
    float scalar = q.w;
    Vector<3, T> imaginary = Vector<3, T>(q.x, q.y, q.z) * (-static_cast<T>(1));

    return Quaternion<T>(scalar, imaginary);
}

template<typename T>
Quaternion<T> Quaternion<T>::inverse(const Quaternion<T>& q)
{
    T abs_value = Quaternion<T>::length(q);
    abs_value *= abs_value;
    abs_value = static_cast<T>(1) / abs_value;

    Quaternion<T> conjugate_value = Quaternion<T>::conjugate(q);

    T scalar = conjugate_value.w * abs_value;
    Vector<3, T> imaginary = Vector<3, T>(conjugate_value.x, conjugate_value.y, conjugate_value.z) * abs_value;

    return Quaternion<T>(scalar, imaginary);
}

template<typename T>
Vector<2, T> Quaternion<T>::rotate(const Quaternion<T>& q, const Vector<2, T>& v)
{
    return q * v;
}

template<typename T>
Vector<3, T> Quaternion<T>::rotate(const Quaternion<T>& q, const Vector<3, T>& v)
{
    return q * v;
}

template<typename T>
Vector<4, T> Quaternion<T>::rotate(const Quaternion<T>& q, const Vector<4, T>& v)
{
    return q * v;
}

template<typename T>
Matrix<4, 4, T> Quaternion<T>::to_matrix4(const Quaternion<T>& q)
{
    Matrix<4, 4, T> result(static_cast<T>(1));

    T qxx(q.x * q.x);
    T qyy(q.y * q.y);
    T qzz(q.z * q.z);
    T qxz(q.x * q.z);
    T qxy(q.x * q.y);
    T qyz(q.y * q.z);
    T qwx(q.w * q.x);
    T qwy(q.w * q.y);
    T qwz(q.w * q.z);

    result[0][0] = static_cast<T>(1) - static_cast<T>(2) * (qyy + qzz);
    result[0][1] = static_cast<T>(2) * (qxy + qwz);
    result[0][2] = static_cast<T>(2) * (qxz - qwy);

    result[1][0] = static_cast<T>(2) * (qxy - qwz);
    result[1][1] = static_cast<T>(1) - static_cast<T>(2) * (qxx +  qzz);
    result[1][2] = static_cast<T>(2) * (qyz + qwx);

    result[2][0] = static_cast<T>(2) * (qxz + qwy);
    result[2][1] = static_cast<T>(2) * (qyz - qwx);
    result[2][2] = static_cast<T>(1) - static_cast<T>(2) * (qxx +  qyy);

    result[3][3] = static_cast<T>(1);

    return result;
}