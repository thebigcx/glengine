#pragma once

#include <memory>

class Scene;
class Material;
class Texture;

/*
    Deserializer is used for loading assets from the /assets directory, complete with the
    associated .meta files for custom import settings.
*/
class Deserializer
{
public:
    static std::shared_ptr<Texture> deserialize_texture(const std::string& path);
    static std::shared_ptr<Material> deserialize_material(const std::string& path);
    static std::shared_ptr<Scene> deserialize_scene(const std::string& path);
};