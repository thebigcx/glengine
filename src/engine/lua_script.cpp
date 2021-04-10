#include "engine/lua_script.h"
#include "engine/mouse.h"
#include "engine/keyboard.h"
#include "engine/lua.h"
#include "engine/lua_api.h"

#include <iostream>
#include <algorithm>

LuaScript::LuaScript()
{
    scripts.push_back(this);
}

LuaScript::LuaScript(const std::string& script)
{
    scripts.push_back(this);
    load_script(script);
}

LuaScript::~LuaScript()
{
    scripts.erase(std::find(scripts.begin(), scripts.end(), this));
}

void LuaScript::on_start()
{
    lua_getglobal(m_lua, "on_start");
    lua_call(m_lua, 0, 0);
}

void LuaScript::on_destroy()
{
    lua_getglobal(m_lua, "on_destroy");
    lua_call(m_lua, 0, 0);
}

void LuaScript::on_update(float dt)
{
    lua_getglobal(m_lua, "on_update");
    lua_pushnumber(m_lua, dt);
    lua_call(m_lua, 1, 0);
}

void LuaScript::load_script(const std::string& script)
{
    m_path = script;

    m_lua = luaL_newstate();
    luaL_openlibs(m_lua);

    if (!luaL_dofile(m_lua, script.c_str()))
    {
        std::cout << lua_tostring(m_lua, -1) << "\n";
    }

    LuaAPI::register_api(*this);
}