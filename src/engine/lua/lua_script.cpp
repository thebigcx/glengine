#include "engine/lua/lua_script.h"
#include "engine/core/mouse.h"
#include "engine/core/keyboard.h"
#include "engine/lua/lua.h"
#include "engine/lua/bindings/lua_api.h"
#include "engine/core/serializer.h"

#include <iostream>
#include <algorithm>

LuaScript::LuaScript()
{
    scripts.push_back(this);
}

LuaScript::LuaScript(const std::string& script)
    : m_path(script)
{
    scripts.push_back(this);
}

LuaScript::~LuaScript()
{
    scripts.erase(std::find(scripts.begin(), scripts.end(), this));
}

void LuaScript::on_start()
{
    m_lua = luaL_newstate();
    luaL_openlibs(m_lua);

    if (luaL_dofile(m_lua, m_path.c_str()))
    {
        std::cout << lua_tostring(m_lua, -1) << "\n";
    }

    for (auto& var : global_vars)
    {
        switch (var.type)
        {
            case LuaGlobalVar::Type::Boolean:
                lua_pushboolean(m_lua, var.boolean);
                lua_setglobal(m_lua, var.name.c_str());
                break;
            case LuaGlobalVar::Type::Number:
                lua_pushnumber(m_lua, var.number);
                lua_setglobal(m_lua, var.name.c_str());
                break;
            case LuaGlobalVar::Type::String:
                lua_pushstring(m_lua, var.string.c_str());
                lua_setglobal(m_lua, var.name.c_str());
                break;
            case LuaGlobalVar::Type::Userdata:
                lua_newuserdata(m_lua, 8);
                lua_setglobal(m_lua, var.name.c_str());
                break;
        };
    }

    LuaAPI::register_api(*this); // TODO: this should go in on_start()

    lua_getglobal(m_lua, "on_start");
    lua_call(m_lua, 0, 0);
}

void LuaScript::on_destroy()
{
    lua_getglobal(m_lua, "on_destroy");
    lua_call(m_lua, 0, 0);

    lua_close(m_lua);
}

void LuaScript::on_update(float dt)
{
    lua_getglobal(m_lua, "on_update");
    lua_pushnumber(m_lua, dt);
    lua_call(m_lua, 1, 0);
}

void LuaScript::set_script(const std::string& script)
{
    m_path = script;
}

void LuaScript::serialize(YAML::Node& node)
{
    Serializer::serialize_lua_script(node, this);
}