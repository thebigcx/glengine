#include "engine/core/serializer.h"
#include "engine/scene/scene.h"
#include "engine/scene/node.h"

#include "yaml-cpp/yaml.h"

#include <fstream>

static void serialize_game_object(YAML::Node node, Node* go)
{
    node[go->get_name()]["Name"] = go->get_name();

    auto transform_node = node[go->get_name()];
    go->get_transform().serialize(transform_node);

    for (auto& c : go->get_components())
    {
        auto go_node = node[go->get_name()]["Components"];
        c.second->serialize(go_node);
    }
}

void Serializer::serialize_scene(const std::shared_ptr<Scene>& scene, const std::string& path)
{
    YAML::Node root;

    auto gos_node = root["Game Objects"];
    
    for (auto& go : scene->get_root_node()->get_children())
    {
        serialize_game_object(gos_node, go);
    }

    std::ofstream fout(path);
    fout << root;
}