#pragma once

#include "engine/application.h"

class Scene;

class Editor : public Application
{
public:
    Editor();

    void on_start() override;
    void on_update(float dt) override;
    void on_destroy() override;

private:
    std::shared_ptr<Scene> m_current_scene = nullptr;
};