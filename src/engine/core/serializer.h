#pragma once

#include <string>
#include <memory>

class Scene;
class Texture;

class Serializer
{
public:
    static void serialize_texture(const std::shared_ptr<Texture>& texture, const std::string& path);
    static void serialize_scene(const std::shared_ptr<Scene>& scene, const std::string& path);
};