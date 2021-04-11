#pragma once

#include <memory>

#include "engine/scene/component.h"

class Material;

class MeshRenderer : public Component
{
public:
    void on_render() override;

    void set_material(const std::shared_ptr<Material>& material) { m_material = material; }
    const std::shared_ptr<Material>& get_material() const { return m_material; }

private:
    std::shared_ptr<Material> m_material = nullptr;
};