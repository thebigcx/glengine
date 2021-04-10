#include "engine/renderer/assets.h"
#include "engine/renderer/texture.h"
#include "engine/renderer/shader.h"
#include "engine/audio/audio.h"

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

    std::shared_ptr<Texture> texture = std::make_shared<Texture>(path);
    m_textures.add(path, texture);
    return texture;
}

std::weak_ptr<Shader> AssetManager::get_shader(const std::string& vs, const std::string& fs)
{
    if (m_shaders.exists(vs))
    {
        return m_shaders.get(vs);
    }

    std::shared_ptr<Shader> shader = std::make_shared<Shader>(vs, fs);
    m_shaders.add(vs, shader);
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