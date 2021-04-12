#include "engine/audio/audio.h"

#include <iostream>

#include <AL/al.h>

#include "engine/audio/dr_libs/dr_flac.h"
#include "engine/audio/dr_libs/dr_mp3.h"
#include "engine/audio/dr_libs/dr_wav.h"

static uint32_t load_mp3(const std::string& path)
{
    drmp3 mp3;
    if (!drmp3_init_file(&mp3, path.c_str(), nullptr))
    {
        std::cout << "Could not open MP3 file: " << path << "\n";
        return 0;
    }

    drmp3_uint64 frame_count;
    drmp3_get_mp3_and_pcm_frame_count(&mp3, nullptr, &frame_count);

    size_t size = frame_count * mp3.channels * sizeof(drmp3_int16);
    drmp3_int16* data = new drmp3_int16[frame_count * mp3.channels];
    drmp3_read_pcm_frames_s16(&mp3, frame_count, data);

    uint32_t buffer;
    alGenBuffers(1, &buffer);
    alBufferData(buffer, AL_FORMAT_STEREO16, data, size, mp3.sampleRate);

    drmp3_uninit(&mp3);
    delete[] data;

    return buffer;
}

static uint32_t load_wav(const std::string& path)
{

}

static uint32_t load_flac(const std::string& path)
{

}

AudioBuffer::AudioBuffer(uint32_t id, const std::string& path)
    : m_id(id), m_path(path)
{

}

AudioBuffer::~AudioBuffer()
{
    alDeleteBuffers(1, &m_id); // TODO: destructor called after Audio::finalize()
}

std::shared_ptr<AudioBuffer> AudioBuffer::load(const std::string& path)
{
    auto extension = path.substr(path.find_last_of(".") + 1);

    if (extension == "mp3")
        return std::make_shared<AudioBuffer>(load_mp3(path), path);
    else
        std::cout << "Unsupported audio file format: " << extension << "\n";
}

