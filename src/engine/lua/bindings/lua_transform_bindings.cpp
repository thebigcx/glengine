#include "engine/lua/bindings/lua_api.h"
#include "engine/renderer/transform.h"

#include "engine/lua/lua.h"

int LuaAPI::lua_get_transform_translation(lua_State* l)
{
    Transform* tr = *(Transform**)lua_touserdata(l, -1);

    create_vector3(l, tr->get_translation().x, tr->get_translation().y, tr->get_translation().z);

    return 1;
}

int LuaAPI::lua_set_transform_translation(lua_State* l)
{
    if (lua_isnumber(l, -1))
    {
        Transform* tr = *(Transform**)lua_touserdata(l, -4);

        float x = lua_tonumber(l, -3);
        float y = lua_tonumber(l, -2);
        float z = lua_tonumber(l, -1);

        tr->set_translation(Vector3f(x, y, z));
    }
    else if (lua_istable(l, -1))
    {
        Transform* tr = *(Transform**)lua_touserdata(l, -2);

        float x = get_vector3_num(l, "x", -2);
        float y = get_vector3_num(l, "y", -2);
        float z = get_vector3_num(l, "z", -2);

        tr->set_translation(Vector3f(x, y, z));
    }

    return 0;
}

int LuaAPI::lua_transform_translate(lua_State* l)
{
    if (lua_isnumber(l, -1))
    {
        Transform* tr = *(Transform**)lua_touserdata(l, -4);

        float x = lua_tonumber(l, -3);
        float y = lua_tonumber(l, -2);
        float z = lua_tonumber(l, -1);

        tr->set_translation(tr->get_translation() + Vector3f(x, y, z));
    }
    else if (lua_istable(l, -1))
    {
        Transform* tr = *(Transform**)lua_touserdata(l, -2);

        float x = get_vector3_num(l, "x", -2);
        float y = get_vector3_num(l, "y", -2);
        float z = get_vector3_num(l, "z", -2);

        tr->set_translation(tr->get_translation() + Vector3f(x, y, z));
    }

    return 0;
}

int LuaAPI::lua_get_transform_rotation(lua_State* l)
{
    Transform* tr = *(Transform**)lua_touserdata(l, -1);

    create_vector3(l, tr->get_rotation().x, tr->get_rotation().y, tr->get_rotation().z);

    return 1;
}

int LuaAPI::lua_set_transform_rotation(lua_State* l)
{
    if (lua_isnumber(l, -1))
    {
        Transform* tr = *(Transform**)lua_touserdata(l, -4);

        float x = lua_tonumber(l, -3);
        float y = lua_tonumber(l, -2);
        float z = lua_tonumber(l, -1);

        tr->set_rotation(Vector3f(x, y, z));
    }
    else if (lua_istable(l, -1))
    {
        Transform* tr = *(Transform**)lua_touserdata(l, -2);

        float x = get_vector3_num(l, "x", -2);
        float y = get_vector3_num(l, "y", -2);
        float z = get_vector3_num(l, "z", -2);

        tr->set_rotation(Vector3f(x, y, z));
    }

    return 0;
}

int LuaAPI::lua_transform_rotate(lua_State* l)
{
    if (lua_isnumber(l, -1))
    {
        Transform* tr = *(Transform**)lua_touserdata(l, -4);

        float x = lua_tonumber(l, -3);
        float y = lua_tonumber(l, -2);
        float z = lua_tonumber(l, -1);

        tr->set_rotation(tr->get_rotation() + Vector3f(x, y, z));
    }
    else if (lua_istable(l, -1))
    {
        Transform* tr = *(Transform**)lua_touserdata(l, -2);

        float x = get_vector3_num(l, "x", -2);
        float y = get_vector3_num(l, "y", -2);
        float z = get_vector3_num(l, "z", -2);

        tr->set_rotation(tr->get_rotation() + Vector3f(x, y, z));
    }

    return 0;
}

int LuaAPI::lua_get_transform_scale(lua_State* l)
{
    Transform* tr = *(Transform**)lua_touserdata(l, -1);

    create_vector3(l, tr->get_scale().x, tr->get_scale().y, tr->get_scale().z);

    return 1;
}

int LuaAPI::lua_set_transform_scale(lua_State* l)
{
    if (lua_isnumber(l, -1))
    {
        Transform* tr = *(Transform**)lua_touserdata(l, -4);

        float x = lua_tonumber(l, -3);
        float y = lua_tonumber(l, -2);
        float z = lua_tonumber(l, -1);

        tr->set_scale(Vector3f(x, y, z));
    }
    else if (lua_istable(l, -1))
    {
        Transform* tr = *(Transform**)lua_touserdata(l, -2);

        float x = get_vector3_num(l, "x", -2);
        float y = get_vector3_num(l, "y", -2);
        float z = get_vector3_num(l, "z", -2);

        tr->set_scale(Vector3f(x, y, z));
    }

    return 0;
}

int LuaAPI::lua_transform_scale(lua_State* l)
{
    if (lua_isnumber(l, -1))
    {
        Transform* tr = *(Transform**)lua_touserdata(l, -4);

        float x = lua_tonumber(l, -3);
        float y = lua_tonumber(l, -2);
        float z = lua_tonumber(l, -1);

        tr->set_scale(tr->get_scale() * Vector3f(x, y, z));
    }
    else if (lua_istable(l, -1))
    {
        Transform* tr = *(Transform**)lua_touserdata(l, -2);

        float x = get_vector3_num(l, "x", -2);
        float y = get_vector3_num(l, "y", -2);
        float z = get_vector3_num(l, "z", -2);

        tr->set_scale(tr->get_scale() * Vector3f(x, y, z));
    }

    return 0;
}

int LuaAPI::lua_get_transform_field(lua_State* l)
{
    Transform* tr = *(Transform**)lua_touserdata(l, -2);
    const char* field = lua_tostring(l, -1);

    lua_getglobal(l, "Transform");
    lua_pushstring(l, field);
    lua_rawget(l, -2);

    return 1;
}

void LuaAPI::register_transform_functions(lua_State* l)
{
    lua_newtable(l);
    lua_pushvalue(l, lua_gettop(l));
    lua_setglobal(l, "Transform");

    lua_pushcfunction(l, lua_get_transform_translation);
    lua_setfield(l, -2, "get_translation");
    lua_pushcfunction(l, lua_set_transform_translation);
    lua_setfield(l, -2, "set_translation");
    lua_pushcfunction(l, lua_transform_translate);
    lua_setfield(l, -2, "translate");

    lua_pushcfunction(l, lua_get_transform_rotation);
    lua_setfield(l, -2, "get_rotation");
    lua_pushcfunction(l, lua_set_transform_rotation);
    lua_setfield(l, -2, "set_rotation");
    lua_pushcfunction(l, lua_transform_rotate);
    lua_setfield(l, -2, "rotate");

    lua_pushcfunction(l, lua_get_transform_scale);
    lua_setfield(l, -2, "get_scale");
    lua_pushcfunction(l, lua_set_transform_scale);
    lua_setfield(l, -2, "set_scale");
    lua_pushcfunction(l, lua_transform_scale);
    lua_setfield(l, -2, "scale");

    luaL_newmetatable(l, "TransformMetaTable");
    lua_pushstring(l, "__index");
    lua_pushcfunction(l, lua_get_transform_field);
    lua_settable(l, -3);
}