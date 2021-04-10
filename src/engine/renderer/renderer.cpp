#include "engine/renderer/renderer.h"
#include "engine/renderer/renderer_2d.h"

#include <iostream>
#include <glad/glad.h>

void GLAPIENTRY message_callback(GLenum source, GLenum type, GLuint id, GLenum severity, GLsizei length, const char* message, const void* userParam)
{
    switch (severity)
    {
        case GL_DEBUG_SEVERITY_HIGH:
            std::cout << "[OpenGL Debug HIGH] " << message << "\n";
            break;
        case GL_DEBUG_SEVERITY_MEDIUM:
            std::cout << "[OpenGL Debug MEDIUM] " << message << "\n";
            break;
        case GL_DEBUG_SEVERITY_LOW:
            std::cout << "[OpenGL Debug LOW] " << message << "\n";
            break;
    }
}

void Renderer::init()
{
    glEnable(GL_DEBUG_OUTPUT);
    glDebugMessageCallback(message_callback, 0);

    Renderer2D::init();
}