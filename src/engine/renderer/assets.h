#pragma once

#include <unordered_map>
#include <string>
#include <memory>

template<typename T>
class AssetCache
{
public:
    ~AssetCache()
    {
        m_assets.clear();
    }

    void add(const std::string& name, const std::shared_ptr<T>& asset)
    {
        m_assets.emplace(std::make_pair(name, asset));
    }

    std::weak_ptr<T> get(const std::string& name)
    {
        return m_assets[name];
    }

    void remove(const std::string& name)
    {
        if (exists(name))
        {
            m_assets.erase(m_assets.find(name));
        }
    }

    bool exists(const std::string& name)
    {
        return m_assets.find(name) != m_assets.end();
    }

    void flush()
    {
        m_assets.clear();
    }

    std::unordered_map<std::string, std::shared_ptr<T>>& get_internal_list() { return m_assets; }

private:
    std::unordered_map<std::string, std::shared_ptr<T>> m_assets;
};

class Texture;
class Shader;
class AudioBuffer;
class Material;
class Scene;

class AssetManager
{
public:
    static AssetManager* get_instance()
    {
        static AssetManager manager;
        return &manager;
    }

    void flush();

    void load_asset_folder(const std::string& path);

    AssetCache<Material>& get_material_cache() { return m_materials; }
    AssetCache<Scene>& get_scene_cache() { return m_scenes; }

    std::weak_ptr<Texture> get_texture(const std::string& path);
    std::weak_ptr<Shader> get_shader(const std::string& path);
    std::weak_ptr<AudioBuffer> get_audio_buffer(const std::string& path);
    std::weak_ptr<Material> get_material(const std::string& name);
    std::weak_ptr<Scene> get_scene(const std::string& name);

    std::weak_ptr<Material> create_material(const std::string& name);
    std::weak_ptr<Scene> create_scene(const std::string& name);

private:
    AssetCache<Texture> m_textures;
    AssetCache<Shader> m_shaders;
    AssetCache<AudioBuffer> m_audio_buffers;
    AssetCache<Material> m_materials;
    AssetCache<Scene> m_scenes;
};