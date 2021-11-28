#include "editor/panels/debug_panel.h"

#include <imgui/imgui.h>

std::string DebugPanel::m_log = "Started\n";

void DebugPanel::imgui_render()
{
    ImGui::Begin("Debug Log");
    ImGui::Text(m_log.c_str());
    ImGui::End();
}

void DebugPanel::write_msg(const std::string& msg)
{
    m_log += msg;
}