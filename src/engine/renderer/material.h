#pragma once

#include <memory>
#include <string>

class Texture;
class Shader;

class Material
{
public:
    Material();

    void set_albedo(const std::weak_ptr<Texture>& albedo) { m_albedo = albedo; }
    void set_shader(const std::weak_ptr<Shader>& shader) { m_shader = shader; }

    const std::weak_ptr<Texture>& get_albedo() const { return m_albedo; }
    const std::weak_ptr<Shader>& get_shader() const { return m_shader; }

    void set_name(const std::string& name) { m_name = name; }
    const std::string& get_name() const { return m_name; }
    
private:
    std::weak_ptr<Texture> m_albedo;
    std::weak_ptr<Shader> m_shader;

    std::string m_name = "";
};