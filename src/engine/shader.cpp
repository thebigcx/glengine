#include "engine/shader.h"

#include <glad/glad.h>

#include <fstream>
#include <iostream>
#include <sstream>

static std::string open_file(const std::string& path)
{
    std::ostringstream stream;

    std::ifstream file(path);
    stream << file.rdbuf();
    return stream.str();
}

Shader::Shader(const std::string& vs, const std::string& fs)
{
    auto v_shader = open_file(vs);
    auto f_shader = open_file(fs);
    const char* v_shader_src = v_shader.c_str();
    const char* f_shader_src = f_shader.c_str();

    unsigned int vertex, fragment;
    int success;
    char info_log[512];

    vertex = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertex, 1, &v_shader_src, nullptr);
    glCompileShader(vertex);

    // Check compile errors
    glGetShaderiv(vertex, GL_COMPILE_STATUS, &success);
    if (!success)
    {
        glGetShaderInfoLog(vertex, 512, nullptr, info_log);
        std::cout << "Failed to compile vertex shader: " << info_log << "\n";
    }

    fragment = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragment, 1, &f_shader_src, nullptr);
    glCompileShader(fragment);

    // Check compile errors
    glGetShaderiv(fragment, GL_COMPILE_STATUS, &success);
    if (!success)
    {
        glGetShaderInfoLog(fragment, 512, nullptr, info_log);
        std::cout << "Failed to compile fragment shader: " << info_log << "\n";
    }

    m_id = glCreateProgram();
    glAttachShader(m_id, vertex);
    glAttachShader(m_id, fragment);
    glLinkProgram(m_id);

    // Check link errors
    glGetProgramiv(m_id, GL_LINK_STATUS, &success);
    if (!success)
    {
        glGetProgramInfoLog(m_id, 512, nullptr, info_log);
        std::cout << "Failed to link shader program: " << info_log << "\n";
    }

    glDeleteShader(vertex);
    glDeleteShader(fragment);
}

Shader::~Shader()
{
    glDeleteProgram(m_id);
}

int Shader::get_uniform_location(const std::string& name)
{
    int location = glGetUniformLocation(m_id, name.c_str());

    if (location == -1)
    {
        std::cout << "Shader does not contain uniform: " << name << "\n";
    }

    return location;
}

void Shader::bind() const
{
    glUseProgram(m_id);
}

void Shader::unbind() const
{
    glUseProgram(0);
}

void Shader::uniform_matrix4(const std::string& name, const Matrix4f& value)
{
    glUniformMatrix4fv(get_uniform_location(name), 1, GL_FALSE, &value[0].x);
}