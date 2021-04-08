#pragma once

class ImFont;

class ImGuiLayer
{
public:
    static void init();
    static void begin_frame();
    static void end_frame();
    static void destroy();

    static inline ImFont* fork_awesome_font;
};