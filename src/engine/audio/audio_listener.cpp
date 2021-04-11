#include "engine/audio/audio.h"
#include "engine/scene/node.h"

#include <AL/al.h>
#include <yaml-cpp/yaml.h>

void AudioListener::set_main(AudioListener& listener)
{
    if (m_main_listener)
        m_main_listener->m_is_main_listener = false;

    listener.m_is_main_listener = true;

    m_main_listener = &listener;

    Vector3f pos = m_main_listener->get_position();
    alListenerfv(AL_POSITION, &pos.x);
}

void AudioListener::on_transform_change()
{
    if (m_is_main_listener)
    {
        Vector3f pos = m_main_listener->get_position();
        alListenerfv(AL_POSITION, &pos.x);
    }
}

Vector3f AudioListener::get_position() const
{
    return m_owner->get_transform().get_world_transform()[3];
}

void AudioListener::serialize(YAML::Node& node)
{
    node["Audio Listener"]["Main"] = m_is_main_listener;
}