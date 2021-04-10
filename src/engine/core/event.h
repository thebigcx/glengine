#pragma once

#include <cstdint>

#include "engine/core/keyboard.h"

enum class EventType
{
    WindowResize, KeyPress
};

class Event
{
public:
    Event(EventType type);
    virtual ~Event() {}

    EventType get_type() const { return m_type; }

    bool handled = false;

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

class KeyPressEvent : public Event
{
public:
    KeyPressEvent(Key key);

    Key get_key() const { return m_key; }

private:
    Key m_key;
};