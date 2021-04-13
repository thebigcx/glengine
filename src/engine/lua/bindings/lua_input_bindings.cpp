#include "engine/lua/bindings/lua_api.h"
#include "engine/core/mouse.h"
#include "engine/core/keyboard.h"

#include "engine/lua/lua.h"

int LuaAPI::lua_is_mouse_pressed(lua_State* l)
{
    int button = lua_tonumber(l, -1);

    lua_pushboolean(l, Mouse::is_mouse_pressed((MouseButton)button));

    return 1;
}

int LuaAPI::lua_get_mouse_position(lua_State* l)
{
    Vector2u pos = Mouse::get_mouse_position();

    lua_newtable(l);

    lua_pushstring(l, "x");
    lua_pushnumber(l, pos.x);
    lua_settable(l, -3);

    lua_pushstring(l, "y");
    lua_pushnumber(l, pos.y);
    lua_settable(l, -3);

    return 1;
}

int LuaAPI::lua_is_key_pressed(lua_State* l)
{
    int key = lua_tonumber(l, -1);

    lua_pushboolean(l, Keyboard::is_key_pressed((Key)key));

    return 1;
}

void LuaAPI::register_input_functions(lua_State* l)
{
    lua_newtable(l);
    lua_pushvalue(l, lua_gettop(l));
    lua_setglobal(l, "Mouse");
    lua_pushcfunction(l, lua_is_mouse_pressed);
    lua_setfield(l, -2, "is_mouse_pressed");
    lua_pushcfunction(l, lua_get_mouse_position);
    lua_setfield(l, -2, "get_mouse_position");

    lua_newtable(l);
    lua_pushcfunction(l, lua_is_key_pressed);
    lua_setfield(l, -2, "is_key_pressed");
    lua_setglobal(l, "Keyboard");
}