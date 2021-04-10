#include "engine/core/event.h"

Event::Event(EventType type)
    : m_type(type)
{

}

WindowResizeEvent::WindowResizeEvent(uint32_t width, uint32_t height)
    : Event(EventType::WindowResize), m_width(width), m_height(height)
{

}

KeyPressEvent::KeyPressEvent(Key key)
    : Event(EventType::KeyPress), m_key(key)
{

}