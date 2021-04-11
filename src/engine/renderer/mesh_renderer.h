#pragma once

#include <memory>

#include "engine/scene/component.h"

class Material;

class MeshRenderer : public Component
{
public:
    void on_render() override;

    void set_material(const std::weak_ptr<Material>& material) { m_material = material; }
    const std::weak_ptr<Material>& get_material() const { return m_material; }

    void serialize(YAML::Node& node) override;

private:
    std::weak_ptr<Material> m_material;
};