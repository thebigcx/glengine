#pragma once

#include <memory>
#include <string>

#include "engine/scene/component.h"

struct ALCcontext;
struct ALCdevice;

class Audio
{
public:
    static void init();
    static void finalize();

private:
    static inline ALCcontext* m_context;
    static inline ALCdevice* m_device;
};

class AudioBuffer
{
public:
    AudioBuffer(uint32_t id, const std::string& path); // results from the static ::load function
    ~AudioBuffer();

    static std::shared_ptr<AudioBuffer> load(const std::string& path);

    uint32_t get_id() const { return m_id; }
    const std::string& get_path() const { return m_path; }

private:
    uint32_t m_id;

    std::string m_path = "";

};

class AudioListener : public Component
{
public:
    static void set_main(const AudioListener& listener);
};

class AudioSource : public Component
{
public:
    enum class State
    {
        Initial, Playing, Paused, Stopped
    };

public:
    AudioSource();
    AudioSource(const std::shared_ptr<AudioBuffer>& buffer);

    void on_destroy() override;
    void on_transform_change() override;

    void set_buffer(const std::shared_ptr<AudioBuffer>& buffer);
    const std::shared_ptr<AudioBuffer>& get_buffer() const { return m_buffer; }

    void play() const;
    void pause() const;
    void rewind() const;
    void stop() const;

    AudioSource::State get_state() const;

private:
    uint32_t m_id;

    std::shared_ptr<AudioBuffer> m_buffer;
};