#include "engine/core/serializer.h"
#include "engine/scene/scene.h"
#include "engine/scene/node.h"
#include "engine/renderer/assets.h"
#include "engine/renderer/material.h"
#include "engine/renderer/texture.h"
#include "engine/renderer/sprite.h"
#include "engine/renderer/camera.h"
#include "engine/audio/audio.h"
#include "engine/lua/lua_script.h"
#include "engine/renderer/mesh.h"
#include "engine/renderer/mesh_renderer.h"

#include "yaml-cpp/yaml.h"

#include <fstream>

void Serializer::serialize_texture(const std::shared_ptr<Texture>& texture, const std::string& path)
{
    YAML::Node root;

    root["Is SRGB"] = texture->is_srgb();

    std::ofstream fout(path);
    fout << root;
}

static void serialize_game_object(YAML::Node node, Node* go)
{
    YAML::Node go_node;
    go_node["Name"] = go->get_name();

    auto transform_node = go_node;
    Serializer::serialize_transform(go_node, go->get_transform());

    for (auto& c : go->get_components())
    {
        auto go_comp_node = go_node["Components"];
        
        c.second->serialize(go_comp_node);
    }

    for (auto& c : go->get_children())
    {
        auto child_node = go_node["Children"];
        serialize_game_object(child_node, c);
    }

    YAML::Node gameobject;
    gameobject["Game Object"] = go_node;
    node.push_back(gameobject);
}

void Serializer::serialize_scene(const std::shared_ptr<Scene>& scene, const std::string& path)
{
    YAML::Node root;

    auto gos_node = root["Game Objects"];
    
    for (auto& go : scene->get_root_node()->get_children())
    {
        serialize_game_object(gos_node, go);
    }

    for (auto& material : AssetManager::get_instance()->get_material_cache().get_internal_list())
    {
        material.second->serialize();
    }

    std::ofstream fout(path);
    fout << root;
}

void Serializer::serialize_sprite(YAML::Node& node, Sprite* sprite)
{
    if (sprite->get_texture().lock())
        node["Sprite"]["Texture"] = sprite->get_texture().lock()->get_path();
    else
        node["Sprite"]["Texture"] = "None";

    node["Sprite"]["Color"][0] = sprite->get_color().x;
    node["Sprite"]["Color"][1] = sprite->get_color().y;
    node["Sprite"]["Color"][2] = sprite->get_color().z;
    node["Sprite"]["Color"].SetStyle(YAML::EmitterStyle::Flow);

    node["Sprite"]["UV 1"][0] = sprite->get_uv1().x;
    node["Sprite"]["UV 1"][1] = sprite->get_uv1().y;
    node["Sprite"]["UV 1"].SetStyle(YAML::EmitterStyle::Flow);

    node["Sprite"]["UV 2"][0] = sprite->get_uv2().x;
    node["Sprite"]["UV 2"][1] = sprite->get_uv2().y;
    node["Sprite"]["UV 2"].SetStyle(YAML::EmitterStyle::Flow);

    node["Sprite"]["Use UVs"] = sprite->using_uvs();
}

void Serializer::serialize_camera(YAML::Node& node, Camera* camera)
{
    node["Camera"]["Main"] = camera->is_main_camera();

    if (camera->get_projection_type() == CameraProjection::Orthographic)
        node["Camera"]["Projection Type"] = "Orthographic";
    else
        node["Camera"]["Projection Type"] = "Perspective";

    node["Camera"]["Orthographic"]["Size"] = camera->get_ortho().size;
    node["Camera"]["Orthographic"]["Near Clip"] = camera->get_ortho().near_clip;
    node["Camera"]["Orthographic"]["Far Clip"] = camera->get_ortho().far_clip;

    node["Camera"]["Perspective"]["FOV"] = camera->get_perspective().fov;
    node["Camera"]["Perspective"]["Near Clip"] = camera->get_perspective().near_clip;
    node["Camera"]["Perspective"]["Far Clip"] = camera->get_perspective().far_clip;
}

void Serializer::serialize_transform(YAML::Node& node, const Transform& transform)
{
    node["Transform"]["Translation"][0] = transform.get_translation().x;
    node["Transform"]["Translation"][1] = transform.get_translation().y;
    node["Transform"]["Translation"][2] = transform.get_translation().z;
    node["Transform"]["Translation"].SetStyle(YAML::EmitterStyle::Flow);

    node["Transform"]["Rotation"][0] = transform.get_rotation().x;
    node["Transform"]["Rotation"][1] = transform.get_rotation().y;
    node["Transform"]["Rotation"][2] = transform.get_rotation().z;
    node["Transform"]["Rotation"].SetStyle(YAML::EmitterStyle::Flow);

    node["Transform"]["Scale"][0] = transform.get_scale().x;
    node["Transform"]["Scale"][1] = transform.get_scale().y;
    node["Transform"]["Scale"][2] = transform.get_scale().z;
    node["Transform"]["Scale"].SetStyle(YAML::EmitterStyle::Flow);
}

void Serializer::serialize_audio_source(YAML::Node& node, AudioSource* audio_source)
{
    if (audio_source->get_buffer())
        node["Audio Source"]["Path"] = audio_source->get_buffer()->get_path();
    else
        node["Audio Source"]["Path"] = "None";
}

void Serializer::serialize_audio_listener(YAML::Node& node, AudioListener* audio_listener)
{
    node["Audio Listener"]["Main"] = audio_listener->is_main_listener();
}

void Serializer::serialize_lua_script(YAML::Node& node, LuaScript* script)
{
    node["Lua Script"]["Path"] = script->get_path();

    for (auto& global : script->global_vars)
    {
        YAML::Node global_node;
        global_node["Name"] = global.name;

        switch (global.type)
        {
            case LuaGlobalVar::Type::Boolean:
                global_node["Value"] = global.boolean; break;
            case LuaGlobalVar::Type::Number:
                global_node["Value"] = global.number; break;
            case LuaGlobalVar::Type::String:
                global_node["Value"] = global.string; break;
        }

        node["Lua Script"]["Global Vars"].push_back(global_node);
    }
}

void Serializer::serialize_mesh(YAML::Node& node, Mesh* mesh)
{
    node["Mesh"]["Path"] = mesh->get_path();
}

void Serializer::serialize_mesh_renderer(YAML::Node& node, MeshRenderer* mesh_renderer)
{
    if (mesh_renderer->get_material().lock())
        node["Mesh Renderer"]["Material"] = mesh_renderer->get_material().lock()->get_name();
    else
        node["Mesh Renderer"]["Material"] = "None";
}