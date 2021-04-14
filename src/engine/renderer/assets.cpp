#include "engine/renderer/assets.h"
#include "engine/renderer/texture.h"
#include "engine/renderer/shader.h"
#include "engine/renderer/material.h"
#include "engine/scene/scene.h"
#include "engine/audio/audio.h"
#include "engine/core/deserializer.h"

#include <filesystem>

void AssetManager::flush()
{
    m_textures.flush();
    m_shaders.flush();
}

void AssetManager::load_asset_folder(const std::string& path)
{
    for (auto& asset : std::filesystem::directory_iterator("assets"))
    {
        auto ext = asset.path().extension();

        if (ext == ".material")
        {
            auto material = Deserializer::deserialize_material(asset.path());
            material->set_name(asset.path().stem());
            m_materials.add(asset.path().stem(), material);
        }
        else if (ext == ".scene")
        {
            auto scene = Deserializer::deserialize_scene(asset.path());
            m_scenes.add(asset.path().stem(), scene);
        }
    }
}

std::weak_ptr<Texture> AssetManager::get_texture(const std::string& path)
{
    if (m_textures.exists(path))
    {
        return m_textures.get(path);
    }

    std::shared_ptr<Texture> texture = Deserializer::deserialize_texture(path);
    m_textures.add(path, texture);
    return texture;
}

std::weak_ptr<Shader> AssetManager::get_shader(const std::string& path)
{
    if (m_shaders.exists(path))
    {
        return m_shaders.get(path);
    }

    std::shared_ptr<Shader> shader = std::make_shared<Shader>(path);
    m_shaders.add(path, shader);
    return shader;
}

std::weak_ptr<AudioBuffer> AssetManager::get_audio_buffer(const std::string& path)
{
    if (m_audio_buffers.exists(path))
    {
        return m_audio_buffers.get(path);
    }

    std::shared_ptr<AudioBuffer> audio_buffer = AudioBuffer::load(path);
    m_audio_buffers.add(path, audio_buffer);
    return audio_buffer;
}

std::weak_ptr<Material> AssetManager::get_material(const std::string& name)
{
    if (m_materials.exists(name))
    {
        return m_materials.get(name);
    }

    if (std::filesystem::exists("assets/" + name + ".material"))
    {
        auto material = Deserializer::deserialize_material(name);
        m_materials.add(name, material);
        return material;
    }

    return std::shared_ptr<Material>(nullptr);
}

std::weak_ptr<Scene> AssetManager::get_scene(const std::string& name)
{
    if (m_scenes.exists(name))
    {
        return m_scenes.get(name);
    }

    if (std::filesystem::exists("assets/" + name + ".scene"))
    {
        auto scene = Deserializer::deserialize_scene(name);
        m_scenes.add(name, std::shared_ptr<Scene>(scene));
        return m_scenes.get(name);
    }

    return std::shared_ptr<Scene>(nullptr);
}

std::weak_ptr<Material> AssetManager::create_material(const std::string& name)
{
    if (m_materials.exists(name))
    {
        return m_materials.get(name);
    }

    auto material = std::make_shared<Material>();
    material->set_name(name); // TODO: might not need name stored in material
    m_materials.add(name, material);
    return material;
}

std::weak_ptr<Scene> AssetManager::create_scene(const std::string& name)
{
    if (m_scenes.exists(name))
    {
        return m_scenes.get(name);
    }

    auto scene = std::make_shared<Scene>();
    m_scenes.add(name, scene);
    return scene;
}