#pragma once

#include <string>

class AssetsPanel
{
private:
    enum class AssetView
    {
        Texture, Audio, Mesh, Material, Shader, Script
    };

public:
    static void imgui_render();

private:
    static inline AssetView m_asset_view;
};