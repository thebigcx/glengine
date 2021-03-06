#include "engine/audio/audio.h"
#include "engine/scene/node.h"
#include "engine/core/serializer.h"

#include <AL/al.h>

AudioSource::AudioSource()
{
    
}

void AudioSource::on_start()
{
    alGenSources(1, &m_id);
    if (m_buffer)
        alSourcei(m_id, AL_BUFFER, m_buffer->get_id());
}

void AudioSource::on_destroy()
{
    alDeleteSources(1, &m_id);
}

void AudioSource::on_transform_change()
{
    Vector3f pos = m_owner->get_transform().get_world_transform()[3]; // get the translation component
    alSourcefv(m_id, AL_POSITION, &pos.x);
}

void AudioSource::set_buffer(const std::shared_ptr<AudioBuffer>& buffer)
{
    m_buffer = buffer;
}

void AudioSource::play() const
{
    alSourcePlay(m_id);
}

void AudioSource::pause() const
{
    alSourcePause(m_id);
}

void AudioSource::rewind() const
{
    alSourceRewind(m_id);
}

void AudioSource::stop() const
{
    alSourceStop(m_id);
}

AudioSource::State AudioSource::get_state() const
{
    ALenum state;
    alGetSourcei(m_id, AL_SOURCE_STATE, &state);

    switch (state)
    {
        case AL_PLAYING:
            return AudioSource::State::Playing;
        case AL_PAUSED:
            return AudioSource::State::Paused;
        case AL_STOPPED:
            return AudioSource::State::Stopped;
        default:
            return AudioSource::State::Initial;
    };
}

void AudioSource::serialize(YAML::Node& node)
{
    Serializer::serialize_audio_source(node, this);
}