#include "engine/renderer/framebuffer.h"

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

void Framebuffer::bind() const
{
    glViewport(0, 0, m_width, m_height);
    glBindFramebuffer(GL_FRAMEBUFFER, m_id);
}

void Framebuffer::unbind() const
{
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void Framebuffer::recreate()
{
    if (m_id != 0)
    {
        glDeleteFramebuffers(1, &m_id);
        glDeleteTextures(1, &m_color_id);
    }

    glCreateFramebuffers(1, &m_id);
    glBindFramebuffer(GL_FRAMEBUFFER, m_id);

    glCreateTextures(GL_TEXTURE_2D, 1, &m_color_id);
    glTextureStorage2D(m_color_id, 1, GL_RGB8, m_width, m_height);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, m_color_id, 0);

    glCreateTextures(GL_TEXTURE_2D, 1, &m_depth_id);
    glTextureStorage2D(m_depth_id, 1, GL_DEPTH24_STENCIL8, m_width, m_height);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, m_depth_id, 0);

    glBindFramebuffer(GL_FRAMEBUFFER, 0);
}