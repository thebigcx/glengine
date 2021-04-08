#pragma once

#include <cmath>
#include <cassert>
#include <cstring>

#include "engine/maths/vector3.h"
#include "engine/maths/vector4.h"
#include "engine/maths/matrix.h"

template<typename T>
class Matrix<4, 4, T>
{
public:
    Matrix<4, 4, T>()
        : m_cells{ Vector<4, T>(0), Vector<4, T>(0), Vector<4, T>(0), Vector<4, T>(0) }
    {}

    Matrix<4, 4, T>(const T& s)
        : m_cells{ Vector<4, T>(s, 0, 0, 0), Vector<4, T>(0, s, 0, 0), Vector<4, T>(0, 0, s, 0), Vector<4, T>(0, 0, 0, s) }
    {}

    Matrix<4, 4, T>(const Matrix<3, 3, T>& m)
        : m_cells{ Vector<4, T>(m[0], 0), Vector<4, T>(m[1], 0), Vector<4, T>(m[2], 0), Vector<4, T>(0, 0, 0, 1) }
    {}

    Matrix<4, 4, T>(const Vector<4, T>& v0, const Vector<4, T>& v1, const Vector<4, T>& v2, const Vector<4, T>& v3)
        : m_cells{ Vector<4, T>(v0), Vector<4, T>(v1), Vector<4, T>(v2), Vector<4, T>(v3) }
    {
        
    }

    Matrix<4, 4, T>(
        const T& m00, const T& m10, const T& m20, const T& m30,
        const T& m01, const T& m11, const T& m21, const T& m31,
        const T& m02, const T& m12, const T& m22, const T& m32,
        const T& m03, const T& m13, const T& m23, const T& m33
    )
        : m_cells{ { m00, m10, m20, m30 }, 
                   { m01, m11, m12, m13 }, 
                   { m02, m12, m22, m32 }, 
                   { m30, m31, m32, m33 } } {}

    Vector<4, T>& operator[](uint32_t i)
    {
        assert(i < 4);
        return this->m_cells[i];
    }

    const Vector<4, T>& operator[](uint32_t i) const
    {
        assert(i < 4);
        return this->m_cells[i];
    }

    Matrix<4, 4, T>& operator=(const Matrix<4, 4, T>& m);

    static Matrix<4, 4, T> translate(const Matrix<4, 4, T>& m, const Vector<3, T>& v);
    static Matrix<4, 4, T> scale(const Matrix<4, 4, T>& m, const Vector<3, T>& v);
    static Matrix<4, 4, T> rotate(const Matrix<4, 4, T>& m, const T& angle, const Vector<3, T>& v);
    static Matrix<4, 4, T> ortho(const T& left, const T& right, const T& bottom, const T& top, const T& near, const T& far);
    static Matrix<4, 4, T> perspective(const T& fovy, const T& aspect, const T& zNear, const T& zFar);
    static Matrix<4, 4, T> look_at(const Vector<3, T>& pos, const Vector<3, T>& object, const Vector<3, T>& up);

private:
    Vector<4, T> m_cells[4];
};

template<typename T>
Matrix<4, 4, T> operator*(const Matrix<4, 4, T>& m, const T& s)
{
    Matrix<4, 4, T> result = m;

    result[0] *= s;
    result[1] *= s;
    result[2] *= s;
    result[3] *= s;

    return result;
}

template<typename T>
Vector<3, T> operator*(const Matrix<4, 4, T>& m, const Vector<3, T>& v)
{
    return Vector<3, T>(
        m[0][0] * v[0] + m[1][0] * v[1] + m[2][0] * v[2],
        m[0][1] * v[0] + m[1][1] * v[1] + m[2][1] * v[2],
        m[0][2] * v[0] + m[1][2] * v[1] + m[2][2] * v[2]
    );
}

template<typename T>
Matrix<4, 4, T> operator*(const Matrix<4, 4, T>& m1, const Matrix<4, 4, T>& m2)
{
    Matrix<4, 4, T> result;

    result[0] = m1[0] * m2[0][0] + m1[1] * m2[0][1] + m1[2] * m2[0][2] + m1[3] * m2[0][3];
    result[1] = m1[0] * m2[1][0] + m1[1] * m2[1][1] + m1[2] * m2[1][2] + m1[3] * m2[1][3];
    result[2] = m1[0] * m2[2][0] + m1[1] * m2[2][1] + m1[2] * m2[2][2] + m1[3] * m2[2][3];
    result[3] = m1[0] * m2[3][0] + m1[1] * m2[3][1] + m1[2] * m2[3][2] + m1[3] * m2[3][3];

    return result;
}

template<typename T>
Vector<4, T> operator*(const Matrix<4, 4, T>& m, const Vector<4, T>& v)
{
    Vector<4, T> result;

    result.x = m[0][0] * v[0] + m[1][0] * v[1] + m[2][0] * v[2] + m[3][0] * v[3];
    result.y = m[0][1] * v[0] + m[1][1] * v[1] + m[2][1] * v[2] + m[3][1] * v[3];
    result.z = m[0][2] * v[0] + m[1][2] * v[1] + m[2][2] * v[2] + m[3][2] * v[3];
    result.w = m[0][3] * v[0] + m[1][3] * v[1] + m[2][3] * v[2] + m[3][3] * v[3];

    return result;
}

template<typename T>
Matrix<4, 4, T>& Matrix<4, 4, T>::operator=(const Matrix<4, 4, T>& m)
{
    memcpy(&m_cells, &m.m_cells, 16 * sizeof(T));
    return *this;
}

template<typename T>
Matrix<4, 4, T> Matrix<4, 4, T>::translate(const Matrix<4, 4, T>& m, const Vector<3, T>& v)
{
    Matrix<4, 4, T> result = m;

    result[3][0] += v.x * m[0][0];
    result[3][1] += v.y * m[1][1];
    result[3][2] += v.z * m[2][2];

    return result;
}

template<typename T>
Matrix<4, 4, T> Matrix<4, 4, T>::scale(const Matrix<4, 4, T>& m, const Vector<3, T>& v)
{
    Matrix<4, 4, T> result = m;

    result[0] *= Vector<4, T>(v, static_cast<T>(1));
    result[1] *= Vector<4, T>(v, static_cast<T>(1));
    result[2] *= Vector<4, T>(v, static_cast<T>(1));

    return result;
}

template<typename T>
Matrix<4, 4, T> Matrix<4, 4, T>::rotate(const Matrix<4, 4, T>& m, const T& angle, const Vector<3, T>& v)
{
    Matrix<4, 4, T> rotate = m;

    T c = std::cos(angle);
    T s = std::sin(angle);
    T omc = static_cast<T>(1) - c;

    Vector<3, T> axis(normalize(v));

    T x = axis.x;
    T y = axis.y;
    T z = axis.z;

    rotate[0][0] = x * x * omc + c;
    rotate[0][1] = y * x * omc + z * s;
    rotate[0][2] = x * z * omc - y * s;

    rotate[1][0] = x * y * omc - z * s;
    rotate[1][1] = y * y * omc + c;
    rotate[1][2] = y * z * omc + x * s;

    rotate[2][0] = x * z * omc + y * s;
    rotate[2][1] = y * z * omc - x * s;
    rotate[2][2] = z * z * omc + c;

    Matrix<4, 4, T> result;
    result[0] = m[0] * rotate[0][0] + m[1] * rotate[0][1] + m[2] * rotate[0][2];
    result[1] = m[0] * rotate[1][0] + m[1] * rotate[1][1] + m[2] * rotate[1][2];
    result[2] = m[0] * rotate[2][0] + m[1] * rotate[2][1] + m[2] * rotate[2][2];
    result[3] = m[3];

    return result;
}

template<typename T>
Matrix<4, 4, T> Matrix<4, 4, T>::ortho(const T& left, const T& right, const T& bottom, const T& top, const T& near, const T& far)
{
    Matrix<4, 4, T> mat(static_cast<T>(1));

    mat[0][0] = static_cast<T>(2) / (right - left);
    mat[1][1] = static_cast<T>(2) / (top - bottom);
    mat[2][2] = static_cast<T>(-2) / (far - near);

    mat[3][0] = -(right + left) / (right - left);
    mat[3][1] = -(top + bottom) / (top - bottom);
    mat[3][2] = -(far + near) / (far - near);

    return mat;
}

template<typename T>
Matrix<4, 4, T> Matrix<4, 4, T>::perspective(const T& fovy, const T& aspect, const T& zNear, const T& zFar)
{
    Matrix<4, 4, T> result;

    result[0][0] = static_cast<T>(1) / (aspect * std::tan(fovy / static_cast<T>(2)));
    result[1][1] = static_cast<T>(1) / std::tan(fovy / static_cast<T>(2));
    result[2][2] = -(zFar + zNear) / (zFar - zNear);
    result[2][3] = -static_cast<T>(1);
    result[3][2] = -(static_cast<T>(2) * zFar * zNear) / (zFar - zNear);

    return result;
}

template<typename T>
Matrix<4, 4, T> Matrix<4, 4, T>::look_at(const Vector<3, T>& pos, const Vector<3, T>& object, const Vector<3, T>& up)
{
    Matrix<4, 4, T> result(static_cast<T>(1));

    Vector<3, T> f = Vector<3, T>::normalize(object - pos);
    Vector<3, T> s = Vector<3, T>::normalize(Vector<3, T>::cross(f, up));
    Vector<3, T> u = Vector<3, T>::cross(s, f);

    result[0][0] = s.x;
    result[1][0] = s.y;
    result[2][0] = s.z;

    result[0][1] = u.x;
    result[1][1] = u.y;
    result[2][1] = u.z;

    result[0][2] = -f.x;
    result[1][2] = -f.y;
    result[2][2] = -f.z;

    result[3][0] = -Vector<3, T>::dot(s, pos);
    result[3][1] = -Vector<3, T>::dot(u, pos);
    result[3][2] =  Vector<3, T>::dot(f, pos);

    return result;
}

typedef Matrix<4, 4, float>        Matrix4f;
typedef Matrix<4, 4, int>          Matrix4i;
typedef Matrix<4, 4, long>         Matrix4l;
typedef Matrix<4, 4, double>       Matrix4d;
typedef Matrix<4, 4, unsigned int> Matrix4u;