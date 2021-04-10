#include "editor/imgui_layer.h"
#include "engine/core/application.h"

#include "imgui/imgui.h"
#include "imgui/imgui_impl_glfw.h"
#include "imgui/imgui_impl_opengl3.h"
#include "imguizmo/ImGuizmo.h"

void ImGuiLayer::init()
{
    ImGui::CreateContext();

    auto& io = ImGui::GetIO();
    io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;

    io.Fonts->AddFontFromFileTTF("assets/font.ttf", 24.0f);

    static ImWchar ranges[] = { 0xf000, 0xf3ff, 0 };
    ImFontConfig config;
    config.MergeMode = true;
    fork_awesome_font = io.Fonts->AddFontFromFileTTF("assets/forkawesome.ttf", 24, &config, ranges);

    ImGui_ImplGlfw_InitForOpenGL(Application::get_instance()->get_window()->get_native_handle(), true);
    ImGui_ImplOpenGL3_Init("#version 460");
}

void ImGuiLayer::begin_frame()
{
    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplGlfw_NewFrame();
    ImGui::NewFrame();
    ImGuizmo::BeginFrame();
}

void ImGuiLayer::end_frame()
{
    ImGui::Render();
    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

    ImGui::UpdatePlatformWindows();
	ImGui::RenderPlatformWindowsDefault();
}

void ImGuiLayer::destroy()
{
    ImGui_ImplGlfw_Shutdown();
    ImGui_ImplOpenGL3_Shutdown();

    ImGui::DestroyContext();
}