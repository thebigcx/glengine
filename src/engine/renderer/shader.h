#pragma once

#include <string>

#include "engine/maths/matrix4.h"

class Shader
{
public:
    Shader(const std::string& path);
    ~Shader();

    int get_uniform_location(const std::string& name);

    void bind() const;
    void unbind() const;

    void uniform_matrix4(const std::string& name, const Matrix4f& value);

    const std::string& get_path() const { return m_path; }

private:
    uint32_t m_id = 0;
    std::string m_path = "";

    void compile(const char* vs, const char* fs);
};