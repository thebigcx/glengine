#pragma once

#include <string>
#include <memory>

class Scene;

class Serializer
{
public:
    static void serialize_scene(const std::shared_ptr<Scene>& scene, const std::string& path);
};