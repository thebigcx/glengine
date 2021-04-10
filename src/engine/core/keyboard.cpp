#include "engine/core/keyboard.h"
#include "engine/core/application.h"

#include <GLFW/glfw3.h>

bool Keyboard::is_key_pressed(Key key)
{
    return glfwGetKey(Application::get_instance()->get_window()->get_native_handle(), (int)key);
}