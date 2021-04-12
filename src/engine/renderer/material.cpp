#include "engine/renderer/material.h"
#include "engine/renderer/texture.h"
#include "engine/renderer/shader.h"

#include <yaml-cpp/yaml.h>
#include <fstream>

Material::Material()
{
    
}

void Material::serialize()
{
    YAML::Node root;

    root["Name"] = m_name;

    if (m_shader.lock())
        root["Shader"] = m_shader.lock()->get_path();
    else
        root["Shader"] = "None";

    if (m_albedo.lock())
        root["Albedo"] = m_albedo.lock()->get_path();
    else
        root["Albedo"] = "None";

    std::ofstream fout("assets/" + m_name + ".material");
    fout << root;
}