#include "engine/renderer/assets.h"
#include "engine/renderer/texture.h"
#include "engine/renderer/shader.h"
#include "engine/renderer/material.h"
#include "engine/audio/audio.h"
#include "engine/core/deserializer.h"

#include <filesystem>

void AssetManager::flush()
{
    m_textures.flush();
    m_shaders.flush();
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