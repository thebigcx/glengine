#pragma once

#include <string>
#include <memory>

namespace YAML
{
class Node;
}

class Scene;
class Texture;
class Sprite;
class Camera;
class AudioSource;
class AudioListener;
class LuaScript;
class Mesh;
class MeshRenderer;
class Transform;

class Serializer
{
public:
    static void serialize_texture(const std::shared_ptr<Texture>& texture, const std::string& path);
    static void serialize_scene(const std::shared_ptr<Scene>& scene, const std::string& path);

    static void serialize_sprite(YAML::Node& node, Sprite* sprite);
    static void serialize_camera(YAML::Node& node, Camera* camera);
    static void serialize_transform(YAML::Node& node, const Transform& transform);
    static void serialize_audio_source(YAML::Node& node, AudioSource* audio_source);
    static void serialize_audio_listener(YAML::Node& node, AudioListener* audio_listener);
    static void serialize_lua_script(YAML::Node& node, LuaScript* script);
    static void serialize_mesh(YAML::Node& node, Mesh* mesh);
    static void serialize_mesh_renderer(YAML::Node& node, MeshRenderer* mesh_renderer);
};