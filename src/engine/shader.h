#pragma once

#include <string>

class Shader
{
public:
    Shader(const std::string& vs, const std::string& fs);
    ~Shader();

private:
    uint32_t m_id = 0;
};