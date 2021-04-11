#pragma once

#include <string>
#include <filesystem>

class AssetView
{
public:
    virtual ~AssetView() {}

    virtual void imgui_render(const std::filesystem::directory_entry& asset) = 0;
};

class TextureView : public AssetView
{
public:
    void imgui_render(const std::filesystem::directory_entry& asset) override;
};

class AudioView : public AssetView
{
public:
    void imgui_render(const std::filesystem::directory_entry& asset) override;
};

class ScriptView : public AssetView
{
public:
    void imgui_render(const std::filesystem::directory_entry& asset) override;
};

class ModelView : public AssetView
{
public:
    void imgui_render(const std::filesystem::directory_entry& asset) override;
};

class AssetsPanel
{
public:
    static void imgui_render();

private:
    static inline AssetView* m_asset_view;
};