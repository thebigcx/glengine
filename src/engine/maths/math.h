#pragma once

#include "engine/maths/vector3.h"

namespace Math
{

static double to_radians(float deg)
{
    return deg * static_cast<float>(0.01745329251994329576923690768489);
}

template<typename T>
static Vector<3, T> to_radians(const Vector<3, T>& v)
{
    return Vector<3, T>(to_radians(v.x), to_radians(v.y), to_radians(v.z));
}


};