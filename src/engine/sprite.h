#pragma once

#include "engine/component.h"

class Sprite : public Component
{
public:
    Sprite() {}

    void on_render() override;
};