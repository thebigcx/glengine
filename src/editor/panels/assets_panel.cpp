#include "editor/panels/assets_panel.h"
#include "editor/imgui_layer.h"
#include "engine/renderer/assets.h"
#include "engine/renderer/texture.h"
#include "engine/renderer/material.h"
#include "editor/panels/inspector_panel.h"
#include "editor/editor.h"

#include "editor/fork_awesome/fork_awesome_icons.h"

#include <imgui/imgui.h>

#include <filesystem>

void TextureView::imgui_render()
{
    for (auto& asset : std::filesystem::directory_iterator("assets"))
    {
        auto ext = asset.path().extension();

        if (ext == ".png" || ext == ".jpeg" || ext == ".jpg" || ext == ".tga")
        {
            if (ImGui::ImageButton(reinterpret_cast<void*>(AssetManager::get_instance()->get_texture(asset.path()).lock()->get_id()), ImVec2{80, 80}, ImVec2{0, 1}, ImVec2{1, 0}))
            {
                InspectorPanel::selection_type = SelectionType::Texture;
                InspectorPanel::texture_selection = asset.path().string();
            }

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
    }
}

void AudioView::imgui_render()
{
    for (auto& asset : std::filesystem::directory_iterator("assets"))
    {
        auto ext = asset.path().extension();

        if (ext == ".mp3" || ext == ".wav" || ext == ".flac")
        {
            ImGui::Text(asset.path().c_str());

            if (ImGui::IsItemHovered())
                ImGui::SetTooltip(asset.path().string().c_str());

            if (ImGui::BeginDragDropSource(ImGuiDragDropFlags_SourceAllowNullID))
            {
                ImGui::SetDragDropPayload("audio_buffer_asset", asset.path().c_str(), asset.path().string().size() + 1);

                ImGui::Text(asset.path().c_str());

                ImGui::EndDragDropSource();
            }
        }
    }
}

void ScriptView::imgui_render()
{
    for (auto& asset : std::filesystem::directory_iterator("assets"))
    {
        auto ext = asset.path().extension();

        if (ext == ".lua")
        {
            ImGui::Text(asset.path().c_str());

            if (ImGui::IsItemHovered())
                ImGui::SetTooltip(asset.path().string().c_str());

            if (ImGui::BeginDragDropSource(ImGuiDragDropFlags_SourceAllowNullID))
            {
                ImGui::SetDragDropPayload("script_asset", asset.path().c_str(), asset.path().string().size() + 1);

                ImGui::Text(asset.path().c_str());

                ImGui::EndDragDropSource();
            }
        }
    }
}

void ModelView::imgui_render()
{
    for (auto& asset : std::filesystem::directory_iterator("assets"))
    {
        auto ext = asset.path().extension();

        if (ext == ".fbx")
        {
            ImGui::Text(asset.path().c_str());

            if (ImGui::IsItemHovered())
                ImGui::SetTooltip(asset.path().string().c_str());

            if (ImGui::BeginDragDropSource(ImGuiDragDropFlags_SourceAllowNullID))
            {
                ImGui::SetDragDropPayload("model_asset", asset.path().c_str(), asset.path().string().size() + 1);

                ImGui::Text(asset.path().c_str());

                ImGui::EndDragDropSource();
            }
        }
    }
}

void MaterialView::imgui_render()
{
    for (auto& material : AssetManager::get_instance()->get_material_cache().get_internal_list())
    {
        if (ImGui::Button(material.second->get_name().c_str()))
        {
            InspectorPanel::selection_type = SelectionType::Material;
            InspectorPanel::material_selection = material.first;
        }

        if (ImGui::IsItemHovered())
            ImGui::SetTooltip(material.second->get_name().c_str());

        if (ImGui::BeginDragDropSource(ImGuiDragDropFlags_SourceAllowNullID))
        {
            ImGui::SetDragDropPayload("material_asset", material.second->get_name().c_str(), material.second->get_name().size() + 1);

            ImGui::Text(material.second->get_name().c_str());

            ImGui::EndDragDropSource();
        }
    }

    if (ImGui::Button("Create New Material"))
    {
        // Generate a unique name
        uint32_t suffix = 0;
        while (AssetManager::get_instance()->get_material_cache().exists("Material." + std::to_string(suffix)))
            suffix++;

        auto material = std::make_shared<Material>();
        material->set_name("Material." + std::to_string(suffix));
        AssetManager::get_instance()->get_material_cache().add(material->get_name(), material);
    }
}

void ShaderView::imgui_render()
{
    for (auto& asset : std::filesystem::directory_iterator("assets"))
    {
        auto ext = asset.path().extension();

        if (ext == ".glsl")
        {
            ImGui::Text(asset.path().c_str());

            if (ImGui::IsItemHovered())
                ImGui::SetTooltip(asset.path().string().c_str());

            if (ImGui::BeginDragDropSource(ImGuiDragDropFlags_SourceAllowNullID))
            {
                ImGui::SetDragDropPayload("shader_asset", asset.path().c_str(), asset.path().string().size() + 1);

                ImGui::Text(asset.path().c_str());

                ImGui::EndDragDropSource();
            }
        }
    }
}

void SceneView::imgui_render()
{
    for (auto& asset : std::filesystem::directory_iterator("assets"))
    {
        ImGui::PushID(asset.path().c_str());
        auto ext = asset.path().extension();

        if (ext == ".scene")
        {
            ImGui::Text(asset.path().c_str());
            ImGui::SameLine();
            if (ImGui::Button("Open"))
            {
                static_cast<Editor*>(Application::get_instance())->open_scene(asset.path().string());
            }
        }
        ImGui::PopID();
    }
}

void AssetsPanel::imgui_render()
{
    if (!m_asset_view)
        m_asset_view = new TextureView();

    ImGui::Begin(ICON_FK_FOLDER_OPEN " Assets");

    // TODO: don't hardcode the size (200, 0)
    ImGui::BeginChild("asset_menu", ImVec2(200, 0));

    if (ImGui::Button(ICON_FK_FILE_IMAGE_O " Textures", ImVec2(200, 0)))
        m_asset_view = new TextureView();

    if (ImGui::Button(ICON_FK_FILE_AUDIO_O " Audio Clips", ImVec2(200, 0)))
        m_asset_view = new AudioView();

    if (ImGui::Button(ICON_FK_CUBE " 3D Models", ImVec2(200, 0)))
        m_asset_view = new ModelView();

    if (ImGui::Button(ICON_FK_CIRCLE " Materials", ImVec2(200, 0)))
        m_asset_view = new MaterialView();
    
    if (ImGui::Button(ICON_FK_EYE " Shaders", ImVec2(200, 0)))
        m_asset_view = new ShaderView();

    if (ImGui::Button(ICON_FK_CODE " Scripts", ImVec2(200, 0)))
        m_asset_view = new ScriptView();

    if (ImGui::Button(ICON_FK_TREE " Scenes", ImVec2(200, 0)))
        m_asset_view = new SceneView();

    ImGui::EndChild();

    ImGui::SameLine();

    ImGui::BeginChild("asset_viewer");

    m_asset_view->imgui_render();

    ImGui::EndChild();

    ImGui::End();
}