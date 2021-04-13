#include "engine/lua/bindings/lua_api.h"

#include "engine/lua/lua.h"

int LuaAPI::lua_vector3_new(lua_State* l)
{
    float x = lua_tonumber(l, -3);
    float y = lua_tonumber(l, -2);
    float z = lua_tonumber(l, -1);

    create_vector3(l, x, y, z);

    return 1;
}

void LuaAPI::create_vector3(lua_State* l, float x, float y, float z)
{
    lua_newtable(l);

    lua_pushstring(l, "x");
    lua_pushnumber(l, x);
    lua_settable(l, -3);

    lua_pushstring(l, "y");
    lua_pushnumber(l, y);
    lua_settable(l, -3);

    lua_pushstring(l, "z");
    lua_pushnumber(l, z);
    lua_settable(l, -3);

    luaL_getmetatable(l, "Vector3MetaTable");
    lua_setmetatable(l, -2);
}

int LuaAPI::lua_vector3_add(lua_State* l)
{
    if (lua_istable(l, -1) && lua_istable(l, -2))
    {
        float x1 = get_vector3_num(l, "x", -3);
        float x2 = get_vector3_num(l, "x", -2);

        float y1 = get_vector3_num(l, "y", -3);
        float y2 = get_vector3_num(l, "y", -2);

        float z1 = get_vector3_num(l, "z", -3);
        float z2 = get_vector3_num(l, "z", -2);

        create_vector3(l, x1 + x2, y1 + y2, z1 + z2);
    }
    else if (lua_istable(l, -1) && lua_isnumber(l, -2))
    {
        float n = lua_tonumber(l, -2);
        float x1 = get_vector3_num(l, "x", -2);
        float y1 = get_vector3_num(l, "y", -2);
        float z1 = get_vector3_num(l, "z", -2);

        create_vector3(l, x1 + n, y1 + n, z1 + n);
    }
    else if (lua_istable(l, -2) && lua_isnumber(l, -1))
    {
        float n = lua_tonumber(l, -1);
        float x1 = get_vector3_num(l, "x", -3);
        float y1 = get_vector3_num(l, "y", -3);
        float z1 = get_vector3_num(l, "z", -3);

        create_vector3(l, x1 + n, y1 + n, z1 + n);
    }

    return 1;
}

int LuaAPI::lua_vector3_sub(lua_State* l)
{
    if (lua_istable(l, -1) && lua_istable(l, -2))
    {
        float x1 = get_vector3_num(l, "x", -3);
        float x2 = get_vector3_num(l, "x", -2);

        float y1 = get_vector3_num(l, "y", -3);
        float y2 = get_vector3_num(l, "y", -2);

        float z1 = get_vector3_num(l, "z", -3);
        float z2 = get_vector3_num(l, "z", -2);

        create_vector3(l, x1 - x2, y1 - y2, z1 - z2);
    }
    else if (lua_istable(l, -1) && lua_isnumber(l, -2))
    {
        float n = lua_tonumber(l, -2);
        float x1 = get_vector3_num(l, "x", -2);
        float y1 = get_vector3_num(l, "y", -2);
        float z1 = get_vector3_num(l, "z", -2);

        create_vector3(l, x1 - n, y1 - n, z1 - n);
    }
    else if (lua_istable(l, -2) && lua_isnumber(l, -1))
    {
        float n = lua_tonumber(l, -1);
        float x1 = get_vector3_num(l, "x", -3);
        float y1 = get_vector3_num(l, "y", -3);
        float z1 = get_vector3_num(l, "z", -3);

        create_vector3(l, x1 - n, y1 - n, z1 - n);
    }

    return 1;
}

int LuaAPI::lua_vector3_mul(lua_State* l)
{
    if (lua_istable(l, -1) && lua_istable(l, -2))
    {
        float x1 = get_vector3_num(l, "x", -3);
        float x2 = get_vector3_num(l, "x", -2);

        float y1 = get_vector3_num(l, "y", -3);
        float y2 = get_vector3_num(l, "y", -2);

        float z1 = get_vector3_num(l, "z", -3);
        float z2 = get_vector3_num(l, "z", -2);

        create_vector3(l, x1 * x2, y1 * y2, z1 * z2);
    }
    else if (lua_istable(l, -1) && lua_isnumber(l, -2))
    {
        float n = lua_tonumber(l, -2);
        float x1 = get_vector3_num(l, "x", -2);
        float y1 = get_vector3_num(l, "y", -2);
        float z1 = get_vector3_num(l, "z", -2);

        create_vector3(l, x1 * n, y1 * n, z1 * n);
    }
    else if (lua_istable(l, -2) && lua_isnumber(l, -1))
    {
        float n = lua_tonumber(l, -1);
        float x1 = get_vector3_num(l, "x", -3);
        float y1 = get_vector3_num(l, "y", -3);
        float z1 = get_vector3_num(l, "z", -3);

        create_vector3(l, x1 * n, y1 * n, z1 * n);
    }

    return 1;
}

int LuaAPI::lua_vector3_div(lua_State* l)
{
    if (lua_istable(l, -1) && lua_istable(l, -2))
    {
        float x1 = get_vector3_num(l, "x", -3);
        float x2 = get_vector3_num(l, "x", -2);

        float y1 = get_vector3_num(l, "y", -3);
        float y2 = get_vector3_num(l, "y", -2);

        float z1 = get_vector3_num(l, "z", -3);
        float z2 = get_vector3_num(l, "z", -2);

        create_vector3(l, x1 / x2, y1 / y2, z1 / z2);
    }
    else if (lua_istable(l, -1) && lua_isnumber(l, -2))
    {
        float n = lua_tonumber(l, -2);
        float x1 = get_vector3_num(l, "x", -2);
        float y1 = get_vector3_num(l, "y", -2);
        float z1 = get_vector3_num(l, "z", -2);

        create_vector3(l, x1 / n, y1 / n, z1 / n);
    }
    else if (lua_istable(l, -2) && lua_isnumber(l, -1))
    {
        float n = lua_tonumber(l, -1);
        float x1 = get_vector3_num(l, "x", -3);
        float y1 = get_vector3_num(l, "y", -3);
        float z1 = get_vector3_num(l, "z", -3);

        create_vector3(l, x1 / n, y1 / n, z1 / n);
    }

    return 1;
}

void LuaAPI::register_vector3_functions(lua_State* l)
{
    lua_newtable(l);
    lua_pushcfunction(l, lua_vector3_new);
    lua_setfield(l, -2, "new");
    lua_setglobal(l, "Vector3");

    luaL_newmetatable(l, "Vector3MetaTable");

    lua_pushstring(l, "__add");
    lua_pushcfunction(l, lua_vector3_add);
    lua_settable(l, -3);

    lua_pushstring(l, "__sub");
    lua_pushcfunction(l, lua_vector3_sub);
    lua_settable(l, -3);

    lua_pushstring(l, "__mul");
    lua_pushcfunction(l, lua_vector3_mul);
    lua_settable(l, -3);

    lua_pushstring(l, "__div");
    lua_pushcfunction(l, lua_vector3_div);
    lua_settable(l, -3);
}