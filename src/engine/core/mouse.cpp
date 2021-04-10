#include "engine/core/mouse.h"
#include "engine/core/application.h"

#include <GLFW/glfw3.h>

bool Mouse::is_mouse_pressed(MouseButton button)
{
    return glfwGetMouseButton(Application::get_instance()->get_window()->get_native_handle(), (int)button);
}

Vector2u Mouse::get_mouse_position()
{
    double x, y;
    glfwGetCursorPos(Application::get_instance()->get_window()->get_native_handle(), &x, &y);
    return Vector2u(x, y);
}
