#pragma once

#include <string>

class DebugPanel
{
public:
    static void imgui_render();
    static void write_msg(const std::string& msg);

private:
    static std::string m_log;
};