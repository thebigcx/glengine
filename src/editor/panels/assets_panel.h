#pragma once

#include <string>
#include <filesystem>

class AssetView
{
public:
    virtual ~AssetView() {}

    virtual void imgui_render() = 0;
};

class TextureView : public AssetView
{
public:
    void imgui_render() override;
};

class AudioView : public AssetView
{
public:
    void imgui_render() override;
};

class ScriptView : public AssetView
{
public:
    void imgui_render() override;
};

class ModelView : public AssetView
{
public:
    void imgui_render() override;
};

class MaterialView : public AssetView
{
public:
    void imgui_render() override;
};

class ShaderView : public AssetView
{
public:
    void imgui_render() override;
};

class SceneView : public AssetView
{
public:
    void imgui_render() override;   
};

class AssetsPanel
{
public:
    static void imgui_render();

private:
    static inline AssetView* m_asset_view;
};