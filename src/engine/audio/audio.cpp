#include "engine/audio/audio.h"

#include <AL/al.h>
#include <AL/alc.h>

void Audio::init()
{
    m_device = alcOpenDevice(nullptr);
    m_context = alcCreateContext(m_device, nullptr);
    alcMakeContextCurrent(m_context);
}

void Audio::finalize()
{
    alcDestroyContext(m_context);
    alcCloseDevice(m_device);
}