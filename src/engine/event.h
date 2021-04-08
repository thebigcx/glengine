#pragma once

#include <cstdint>

enum class EventType
{
    WindowResize
};

class Event
{
public:
    Event(EventType type);
    virtual ~Event() {}

    EventType get_type() const { return m_type; }

private:
    EventType m_type;
};

class WindowResizeEvent : public Event
{
public:
    WindowResizeEvent(uint32_t width, uint32_t height);

    uint32_t get_width() const { return m_width; }
    uint32_t get_height() const { return m_height; }

private:
    uint32_t m_width, m_height;
};