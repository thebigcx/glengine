#include "editor/panels/assets_panel.h"
#include "editor/imgui_layer.h"
#include "engine/renderer/assets.h"
#include "engine/renderer/texture.h"

#include "editor/fork_awesome/fork_awesome_icons.h"

#include <imgui/imgui.h>

#include <filesystem>

void AssetsPanel::imgui_render()
{
    ImGui::Begin(ICON_FK_FOLDER_OPEN " Assets");

    // TODO: don't hardcode the size (200, 0)
    ImGui::BeginChild("asset_menu", ImVec2(200, 0));

    if (ImGui::Button(ICON_FK_FILE_IMAGE_O " Textures", ImVec2(200, 0)))
        m_asset_view = AssetView::Texture;

    if (ImGui::Button(ICON_FK_FILE_AUDIO_O " Audio Clips", ImVec2(200, 0)))
        m_asset_view = AssetView::Audio;

    if (ImGui::Button(ICON_FK_CUBE " Meshes", ImVec2(200, 0)))
        m_asset_view = AssetView::Mesh;

    if (ImGui::Button(ICON_FK_CIRCLE " Materials", ImVec2(200, 0)))
        m_asset_view = AssetView::Material;

    if (ImGui::Button(ICON_FK_EYE " Shaders", ImVec2(200, 0)))
        m_asset_view = AssetView::Shader;

    if (ImGui::Button(ICON_FK_CODE " Scripts", ImVec2(200, 0)))
        m_asset_view = AssetView::Script;

    ImGui::EndChild();

    ImGui::SameLine();

    ImGui::BeginChild("asset_viewer");

    for (auto& asset : std::filesystem::directory_iterator("assets"))
    {
        auto ext = asset.path().extension();

        if (m_asset_view == AssetView::Texture && (ext == ".png" || ext == ".jpeg" || ext == ".jpg"))
        {
            ImGui::Image(reinterpret_cast<void*>(AssetManager::get_instance()->get_texture(asset.path()).lock()->get_id()), ImVec2{80, 80}, ImVec2{0, 1}, ImVec2{1, 0});

            if (ImGui::IsItemHovered())
            {
                ImGui::SetTooltip(asset.path().string().c_str());
            }

            if (ImGui::BeginDragDropSource(ImGuiDragDropFlags_SourceAllowNullID))
            {
                ImGui::SetDragDropPayload("texture_asset", asset.path().c_str(), asset.path().string().size() + 1);

                ImGui::Image(reinterpret_cast<void*>(AssetManager::get_instance()->get_texture(asset.path()).lock()->get_id()), ImVec2{80, 80}, ImVec2{0, 1}, ImVec2{1, 0});
                ImGui::SameLine();
                ImGui::Text(asset.path().c_str());

                ImGui::EndDragDropSource();
            }

            ImGui::SameLine();
        }

        if (m_asset_view == AssetView::Audio && (ext == ".mp3" || ext == ".wav" || ext == ".flac"))
        {
            ImGui::Image(0, ImVec2{80, 80}, ImVec2{0, 1}, ImVec2{1, 0});

            if (ImGui::IsItemHovered())
            {
                ImGui::SetTooltip(asset.path().string().c_str());
            }

            if (ImGui::BeginDragDropSource(ImGuiDragDropFlags_SourceAllowNullID))
            {
                ImGui::SetDragDropPayload("audio_buffer_asset", asset.path().c_str(), asset.path().string().size() + 1);

                ImGui::Image(0, ImVec2{80, 80}, ImVec2{0, 1}, ImVec2{1, 0});
                ImGui::SameLine();
                ImGui::Text(asset.path().c_str());

                ImGui::EndDragDropSource();
            }

            ImGui::SameLine();
        }

        if (m_asset_view == AssetView::Script && ext == ".lua")
        {
            ImGui::Image(0, ImVec2{80, 80}, ImVec2{0, 1}, ImVec2{1, 0});

            if (ImGui::IsItemHovered())
            {
                ImGui::SetTooltip(asset.path().string().c_str());
            }

            if (ImGui::BeginDragDropSource(ImGuiDragDropFlags_SourceAllowNullID))
            {
                ImGui::SetDragDropPayload("script_asset", asset.path().c_str(), asset.path().string().size() + 1);

                ImGui::Image(0, ImVec2{80, 80}, ImVec2{0, 1}, ImVec2{1, 0});
                ImGui::SameLine();
                ImGui::Text(asset.path().c_str());

                ImGui::EndDragDropSource();
            }

            ImGui::SameLine();
        }
    }

    ImGui::EndChild();

    ImGui::End();
}