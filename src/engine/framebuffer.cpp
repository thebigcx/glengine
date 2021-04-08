#include "engine/framebuffer.h"

#include <glad/glad.h>

Framebuffer::Framebuffer(uint32_t width, uint32_t height)
    : m_width(width), m_height(height)
{
    recreate();
}

Framebuffer::~Framebuffer()
{
    glDeleteFramebuffers(1, &m_id);
}

void Framebuffer::resize(uint32_t width, uint32_t height)
{
    m_width = width;
    m_height = height;
    recreate();
}

void Framebuffer::recreate()
{
    if (m_id != 0)
    {
        glDeleteFramebuffers(1, &m_id);
    }

    glCreateFramebuffers(1, &m_id);

    
}