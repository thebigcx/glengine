#include "engine/core/deserializer.h"
#include "engine/scene/scene.h"
#include "engine/scene/node.h"
#include "engine/renderer/sprite.h"
#include "engine/renderer/assets.h"
#include "engine/renderer/camera.h"
#include "engine/lua/lua_script.h"
#include "engine/audio/audio.h"
#include "engine/renderer/mesh.h"
#include "engine/renderer/material.h"
#include "engine/renderer/mesh_renderer.h"
#include "engine/renderer/texture.h"

#include <yaml-cpp/yaml.h>
#include <iostream>
#include <filesystem>

std::shared_ptr<Texture> Deserializer::deserialize_texture(const std::string& path)
{
    if (!std::filesystem::exists(path + ".meta"))
    {
        return std::make_shared<Texture>(path);
    }

    YAML::Node root = YAML::LoadFile(path + ".meta");

    bool is_srgb = root["Is SRGB"].as<bool>();

    return std::make_shared<Texture>(path, is_srgb);
}

std::shared_ptr<Material> Deserializer::deserialize_material(const std::string& name)
{
    YAML::Node root = YAML::LoadFile(name);

    auto material = std::make_shared<Material>();

    if (root["Shader"].as<std::string>() != "None")
        material->set_shader(AssetManager::get_instance()->get_shader(root["Shader"].as<std::string>()));
    
    if (root["Albedo"].as<std::string>() != "None")
        material->set_albedo(AssetManager::get_instance()->get_texture(root["Albedo"].as<std::string>()));

    return material;
}

static void deserialize_gameobject(YAML::Node node, Node* parent)
{
    Node* go = parent->create_child(node["Name"].as<std::string>());

    // Transform

    Vector3f translation;
    translation.x = node["Transform"]["Translation"][0].as<float>();
    translation.y = node["Transform"]["Translation"][1].as<float>();
    translation.z = node["Transform"]["Translation"][2].as<float>();

    Vector3f rotation;
    rotation.x = node["Transform"]["Rotation"][0].as<float>();
    rotation.y = node["Transform"]["Rotation"][1].as<float>();
    rotation.z = node["Transform"]["Rotation"][2].as<float>();

    Vector3f scale;
    scale.x = node["Transform"]["Scale"][0].as<float>();
    scale.y = node["Transform"]["Scale"][1].as<float>();
    scale.z = node["Transform"]["Scale"][2].as<float>();

    go->get_transform().set_translation(translation);
    go->get_transform().set_rotation(rotation);
    go->get_transform().set_scale(scale);

    // Sprite

    if (node["Components"]["Sprite"])
    {
        Sprite* sprite = go->create_component<Sprite>();

        if (node["Components"]["Sprite"]["Texture"].as<std::string>() != "None")
            sprite->set_texture(AssetManager::get_instance()->get_texture(node["Components"]["Sprite"]["Texture"].as<std::string>()));

        Vector3f color;
        color.x = node["Components"]["Sprite"]["Color"][0].as<float>();
        color.y = node["Components"]["Sprite"]["Color"][1].as<float>();
        color.z = node["Components"]["Sprite"]["Color"][2].as<float>();

        sprite->set_color(color);

        Vector2f uv1, uv2;
        uv1.x = node["Components"]["Sprite"]["UV 1"][0].as<float>();
        uv1.y = node["Components"]["Sprite"]["UV 1"][1].as<float>();

        uv2.x = node["Components"]["Sprite"]["UV 2"][0].as<float>();
        uv2.y = node["Components"]["Sprite"]["UV 2"][1].as<float>();

        sprite->set_uv1(uv1);
        sprite->set_uv2(uv2);

        sprite->use_uvs(node["Components"]["Sprite"]["Use UVs"].as<bool>());
    }

    if (node["Components"]["Camera"])
    {
        Camera* cam = go->create_component<Camera>();

        if (node["Components"]["Camera"]["Main"].as<bool>())
            Camera::set_main_camera(cam);

        float ortho_size = node["Components"]["Camera"]["Orthographic"]["Size"].as<float>();
        float ortho_near_clip = node["Components"]["Camera"]["Orthographic"]["Near Clip"].as<float>();
        float ortho_far_clip = node["Components"]["Camera"]["Orthographic"]["Far Clip"].as<float>();

        float perspective_fov = node["Components"]["Camera"]["Perspective"]["FOV"].as<float>();
        float perspective_near_clip = node["Components"]["Camera"]["Perspective"]["Near Clip"].as<float>();
        float perspective_far_clip = node["Components"]["Camera"]["Perspective"]["Far Clip"].as<float>();

        cam->set_ortho(ortho_size, ortho_near_clip, ortho_far_clip);
        cam->set_perspective(perspective_fov, perspective_near_clip, perspective_far_clip);

        std::string projection_type = node["Components"]["Camera"]["Projection Type"].as<std::string>();
        if (projection_type == "Orthographic")
            cam->set_projection_type(CameraProjection::Orthographic);
        else
            cam->set_projection_type(CameraProjection::Perspective);
    }

    if (node["Components"]["Lua Script"])
    {
        LuaScript* script = go->create_component<LuaScript>();

        script->set_script(node["Components"]["Lua Script"]["Path"].as<std::string>());
    }

    if (node["Components"]["Audio Source"])
    {
        AudioSource* audio = go->create_component<AudioSource>();

        audio->set_buffer(AudioBuffer::load(node["Components"]["Audio Source"]["Path"].as<std::string>()));
    }

    if (node["Components"]["Audio Listener"])
    {
        AudioListener* listener = go->create_component<AudioListener>();

        if (node["Components"]["Audio Listener"]["Main"])
            AudioListener::set_main(*listener);
    }

    if (node["Components"]["Mesh"])
    {
        Mesh* mesh = go->create_component<Mesh>();

        mesh->load(node["Components"]["Mesh"]["Path"].as<std::string>());
    }

    if (node["Components"]["Mesh Renderer"])
    {
        MeshRenderer* renderer = go->create_component<MeshRenderer>();

        auto material = node["Components"]["Mesh Renderer"]["Material"].as<std::string>();

        if (material != "None")
            renderer->set_material(AssetManager::get_instance()->get_material(material));
    }

    // Children

    if (node["Children"])
    {
        for (size_t i = 0; i < node["Children"].size(); i++)
        {
            deserialize_gameobject(node["Children"][i]["Game Object"], go);
        }
    }
}

std::shared_ptr<Scene> Deserializer::deserialize_scene(const std::string& path)
{
    YAML::Node root = YAML::LoadFile(path);

    std::shared_ptr<Scene> scene = std::make_shared<Scene>();

    for (size_t i = 0; i < root["Game Objects"].size(); i++)
    {
        deserialize_gameobject(root["Game Objects"][i]["Game Object"], scene->get_root_node());
    }

    return scene;
}