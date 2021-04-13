#include "engine/core/serializer.h"
#include "engine/scene/scene.h"
#include "engine/scene/node.h"
#include "engine/renderer/assets.h"
#include "engine/renderer/material.h"
#include "engine/renderer/texture.h"

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
    go->get_transform().serialize(go_node);

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

void Serializer::serialize_scene(Scene* scene, const std::string& path)
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