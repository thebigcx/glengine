#pragma once

#include <memory>

class Scene;
class Material;

class Deserializer
{
public:
    static std::shared_ptr<Material> deserialize_material(const std::string& path);
    static std::shared_ptr<Scene> deserialize_scene(const std::string& path);
};