#pragma once

#include <string>

#include "engine/maths/matrix4.h"

class Shader
{
public:
    Shader(const std::string& vs, const std::string& fs);
    ~Shader();

    int get_uniform_location(const std::string& name);

    void bind() const;
    void unbind() const;

    void uniform_matrix4(const std::string& name, const Matrix4f& value);

private:
    uint32_t m_id = 0;
};