#pragma once

#include "engine/maths/vector2.h"

enum class MouseButton
{
    Left = 0,
    Right = 1,
    Middle = 2,
    Last = 8,

    One = 0,
    Two = 1,
    Three = 2,
    Four = 3,
    Five = 4,
    Six = 5,
    Seven = 6,
    Eight = 7
};

class Mouse
{
public:
    static bool is_mouse_pressed(MouseButton button);

    static Vector2u get_mouse_position();
};