#pragma once

#include <cmath>
#include <cassert>
#include <cstring>

#include "engine/maths/vector3.h"
#include "engine/maths/vector4.h"
#include "engine/maths/matrix.h"
#include "engine/maths/epsilon.h"

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
    static Matrix<4, 4, T> inverse(const Matrix<4, 4, T>& mat);

    static void decompose_transform(const Matrix<4, 4, T>& transform, Vector<3, T>& translation, Vector<3, T>& rotation, Vector<3, T>& scale);

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

    Vector<3, T> axis(Vector3f::normalize(v));

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
    Matrix<4, 4, T> result(static_cast<T>(0));

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

// Invert a matrix. I don't know how this works.
template<typename T>
Matrix<4, 4, T> Matrix<4, 4, T>::inverse(const Matrix<4, 4, T>& m)
{
    T coef00 = m[2][2] * m[3][3] - m[3][2] * m[2][3];
    T coef02 = m[1][2] * m[3][3] - m[3][2] * m[1][3];
    T coef03 = m[1][2] * m[2][3] - m[2][2] * m[1][3];

    T coef04 = m[2][1] * m[3][3] - m[3][1] * m[2][3];
    T coef06 = m[1][1] * m[3][3] - m[3][1] * m[1][3];
    T coef07 = m[1][1] * m[2][3] - m[2][1] * m[1][3];

    T coef08 = m[2][1] * m[3][2] - m[3][1] * m[2][2];
    T coef10 = m[1][1] * m[3][2] - m[3][1] * m[1][2];
    T coef11 = m[1][1] * m[2][2] - m[2][1] * m[1][2];

    T coef12 = m[2][0] * m[3][3] - m[3][0] * m[2][3];
    T coef14 = m[1][0] * m[3][3] - m[3][0] * m[1][3];
    T coef15 = m[1][0] * m[2][3] - m[2][0] * m[1][3];

    T coef16 = m[2][0] * m[3][2] - m[3][0] * m[2][2];
    T coef18 = m[1][0] * m[3][2] - m[3][0] * m[1][2];
    T coef19 = m[1][0] * m[2][2] - m[2][0] * m[1][2];

    T coef20 = m[2][0] * m[3][1] - m[3][0] * m[2][1];
    T coef22 = m[1][0] * m[3][1] - m[3][0] * m[1][1];
    T coef23 = m[1][0] * m[2][1] - m[2][0] * m[1][1];

    Vector<4, T> Fac0(coef00, coef00, coef02, coef03);
    Vector<4, T> Fac1(coef04, coef04, coef06, coef07);
    Vector<4, T> Fac2(coef08, coef08, coef10, coef11);
    Vector<4, T> Fac3(coef12, coef12, coef14, coef15);
    Vector<4, T> Fac4(coef16, coef16, coef18, coef19);
    Vector<4, T> Fac5(coef20, coef20, coef22, coef23);

    Vector<4, T> Vec0(m[1][0], m[0][0], m[0][0], m[0][0]);
    Vector<4, T> Vec1(m[1][1], m[0][1], m[0][1], m[0][1]);
    Vector<4, T> Vec2(m[1][2], m[0][2], m[0][2], m[0][2]);
    Vector<4, T> Vec3(m[1][3], m[0][3], m[0][3], m[0][3]);

    Vector<4, T> Inv0(Vec1 * Fac0 - Vec2 * Fac1 + Vec3 * Fac2);
    Vector<4, T> Inv1(Vec0 * Fac0 - Vec2 * Fac3 + Vec3 * Fac4);
    Vector<4, T> Inv2(Vec0 * Fac1 - Vec1 * Fac3 + Vec3 * Fac5);
    Vector<4, T> Inv3(Vec0 * Fac2 - Vec1 * Fac4 + Vec2 * Fac5);

    Vector<4, T> SignA(+1, -1, +1, -1);
    Vector<4, T> SignB(-1, +1, -1, +1);
    Matrix<4, 4, T> Inverse(Inv0 * SignA, Inv1 * SignB, Inv2 * SignA, Inv3 * SignB);

    Vector<4, T> Row0(Inverse[0][0], Inverse[1][0], Inverse[2][0], Inverse[3][0]);

    Vector<4, T> Dot0(m[0] * Row0);
    T Dot1 = (Dot0.x + Dot0.y) + (Dot0.z + Dot0.w);

    T OneOverDeterminant = static_cast<T>(1) / Dot1;

    return Inverse * OneOverDeterminant;
}

template<typename T>
void Matrix<4, 4, T>::decompose_transform(const Matrix<4, 4, T>& transform, Vector<3, T>& translation, Vector<3, T>& rotation, Vector<3, T>& scale)
{
    Matrix<4, 4, T> local_matrix(transform);

    if (epsilon_equal(local_matrix[3][3], static_cast<T>(0), epsilon<T>()))
    {
        return;
    }

    if (
        epsilon_not_equal(local_matrix[0][3], static_cast<T>(0), epsilon<T>()) ||
        epsilon_not_equal(local_matrix[1][3], static_cast<T>(0), epsilon<T>()) ||
        epsilon_not_equal(local_matrix[2][3], static_cast<T>(0), epsilon<T>())
    )
    {
        local_matrix[0][3] = local_matrix[1][3] = local_matrix[2][3] = static_cast<T>(0);
        local_matrix[3][3] = static_cast<T>(1);
    }

    translation = Vector<3, T>(local_matrix[3]);
    local_matrix[3] = Vector<4, T>(static_cast<T>(0), static_cast<T>(0), static_cast<T>(0), local_matrix[3].w);

    Vector<3, T> row[3];

    for (unsigned int x = 0; x < 3; x++)
    for (unsigned int y = 0; y < 3; y++)
    {
        row[x][y] = local_matrix[x][y];
    }

    scale.x = Vector<3, T>::length(row[0]);
    row[0] = Vector<3, T>::scale(row[0], static_cast<T>(1));
    scale.y = Vector<3, T>::length(row[1]);
    row[1] = Vector<3, T>::scale(row[1], static_cast<T>(1));
    scale.z = Vector<3, T>::length(row[2]);
    row[2] = Vector<3, T>::scale(row[2], static_cast<T>(1));

    rotation.y = std::asin(-row[0][2]);
    if (std::cos(rotation.y) != 0)
    {
        rotation.x = std::atan2(row[1][2], row[2][2]);
        rotation.z = std::atan2(row[0][1], row[0][0]);
    }
    else
    {
        rotation.x = std::atan2(-row[2][0], row[1][1]);
        rotation.z = static_cast<T>(0);
    }
    
}

typedef Matrix<4, 4, float>        Matrix4f;
typedef Matrix<4, 4, int>          Matrix4i;
typedef Matrix<4, 4, long>         Matrix4l;
typedef Matrix<4, 4, double>       Matrix4d;
typedef Matrix<4, 4, unsigned int> Matrix4u;