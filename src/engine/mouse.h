#pragma once

#include "engine/maths/vector2.h"

class Mouse
{
public:
    static bool is_mouse_pressed(int button);

    static Vector2u get_mouse_position();
};