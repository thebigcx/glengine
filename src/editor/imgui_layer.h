#pragma once

class ImGuiLayer
{
public:
    static void init();
    static void begin_frame();
    static void end_frame();
    static void destroy();
};