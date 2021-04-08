#include "engine/assets.h"
#include "engine/texture.h"
#include "engine/shader.h"

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