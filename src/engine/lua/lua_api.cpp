#include "engine/lua/lua_api.h"
#include "engine/lua/lua_script.h"
#include "engine/lua/lua.h"
#include "engine/scene/node.h"
#include "engine/core/mouse.h"
#include "engine/core/keyboard.h"
#include "engine/renderer/camera.h"
#include "engine/scene/scene.h"
#include "engine/audio/audio.h"
#include "engine/renderer/sprite.h"

#include <iostream>

static void dumpstack (lua_State *L) {
  int top=lua_gettop(L);
  for (int i=1; i <= top; i++) {
    printf("%d\t%s\t", i, luaL_typename(L,i));
    switch (lua_type(L, i)) {
      case LUA_TNUMBER:
        printf("%g\n",lua_tonumber(L,i));
        break;
      case LUA_TSTRING:
        printf("%s\n",lua_tostring(L,i));
        break;
      case LUA_TBOOLEAN:
        printf("%s\n", (lua_toboolean(L, i) ? "true" : "false"));
        break;
      case LUA_TNIL:
        printf("%s\n", "nil");
        break;
      default:
        printf("%p\n",lua_topointer(L,i));
        break;
    }
  }
}

int LuaAPI::lua_dumpstack(lua_State* l)
{
    dumpstack(l);
    return 0;
}

float LuaAPI::get_vector3_num(lua_State* l, const std::string& field, int table)
{
    lua_pushstring(l, field.c_str());
    lua_gettable(l, table);
    float n = lua_tonumber(l, -1);
    lua_pop(l, 1);

    return n;
}

Node* LuaAPI::find_node(lua_State* l)
{
    for (auto& script : LuaScript::scripts)
    {
        if (script->get_lua_state() == l)
        {
            return script->get_owner();
        }
    }

    std::cout << "Could not find node\n";
    return nullptr;
}

Camera* LuaAPI::find_main_camera()
{
    // TODO: recursive

    // TODO: this is hack, find a better way.
    Scene* scene = LuaScript::scripts[0]->get_owner()->get_scene();

    for (auto& node : scene->get_root_node()->get_children())
    {
        if (node->has_component<Camera>() && node->get_component<Camera>()->is_main_camera())
        {
            return node->get_component<Camera>();
        }
    }

    std::cout << "No main camera in scene\n";
    return nullptr;
}

int LuaAPI::lua_get_this(lua_State* l)
{
    Node* node = find_node(l);
    lua_pushlightuserdata(l, (void*)node);

    luaL_getmetatable(l, "GameObjectMetaTable");
    lua_setmetatable(l, -2);

    return 1;
}

int LuaAPI::lua_get_gameobject_field(lua_State* l)
{
    Node* node = (Node*)lua_touserdata(l, -2);
    const char* field = lua_tostring(l, -1);

    if (!strcmp(field, "transform"))
    {
        lua_pushlightuserdata(l, (void*)&node->get_transform());

        luaL_getmetatable(l, "TransformMetaTable");
        lua_setmetatable(l, -2);

        return 1;
    }

    lua_getglobal(l, "GameObject");
    lua_pushstring(l, field);
    lua_rawget(l, -2);

    return 1;
}

int LuaAPI::lua_get_component(lua_State* l)
{
    Node* node = static_cast<Node*>(lua_touserdata(l, -2));
    const char* component = lua_tostring(l, -1);

    if (!strcmp(component, "Sprite"))
    {
        Sprite* com = node->get_component<Sprite>();
        lua_pushlightuserdata(l, (void*)com);
        luaL_getmetatable(l, "SpriteMetaTable");
        lua_setmetatable(l, -2);

        return 1;
    }
    else if (!strcmp(component, "AudioSource"))
    {
        AudioSource* com = node->get_component<AudioSource>();
        lua_pushlightuserdata(l, (void*)com);
        luaL_getmetatable(l, "AudioSourceMetaTable");
        lua_setmetatable(l, -2);

        return 1;
    }
}

int LuaAPI::lua_has_component(lua_State* l)
{
    Node* node = static_cast<Node*>(lua_touserdata(l, -2));
    const char* component = lua_tostring(l, -1);

    if (!strcmp(component, "Sprite"))
        lua_pushboolean(l, node->has_component<Sprite>());
    else if (!strcmp(component, "AudioSource"))
        lua_pushboolean(l, node->has_component<AudioSource>());
    else if (!strcmp(component, "AudioListener"))
        lua_pushboolean(l, node->has_component<AudioListener>());
    else if (!strcmp(component, "Script"))
        lua_pushboolean(l, node->has_component<LuaScript>());
    else if (!strcmp(component, "Camera"))
        lua_pushboolean(l, node->has_component<Camera>());

    return 1;
}

int LuaAPI::lua_find_gameobject_name(lua_State* l)
{
    /*const char* name = lua_tostring(l, -1);

    Node* node = Node::find_by_name(name);

    lua_pushlightuserdata(l, (void*)node);
    luaL_getmetatable(l, "GameObjectMetaTable");
    lua_setmetatable(l, -2);

    return 1;*/
}

int LuaAPI::lua_gameobject_create(lua_State* l)
{
    return 0;
}

int LuaAPI::lua_gameobject_delete(lua_State* l)
{
    return 0;
}

int LuaAPI::lua_gameobject_get_child(lua_State* l)
{
    Node* parent = (Node*)lua_touserdata(l, -2);
    const char* child_name = lua_tostring(l, -1);

    lua_pushlightuserdata(l, (void*)parent->find_child(child_name));
    luaL_getmetatable(l, "GameObjectMetaTable");
    lua_setmetatable(l, -2);

    return 1;
}

int LuaAPI::lua_gameobject_create_child(lua_State* l)
{
    Node* parent = (Node*)lua_touserdata(l, -2);
    const char* child_name = lua_tostring(l, -1);

    Node* child = parent->create_child(child_name);

    lua_pushlightuserdata(l, (void*)child);
    luaL_getmetatable(l, "GameObjectMetaTable");
    lua_setmetatable(l, -2);

    return 1;
}

int LuaAPI::lua_create_component(lua_State* l)
{
    Node* node = (Node*)lua_touserdata(l, -2);
    const char* component = lua_tostring(l, -1);

    if (!strcmp(component, "Sprite"))
    {
        Sprite* sprite = node->create_component<Sprite>();
        lua_pushlightuserdata(l, (void*)sprite);
        luaL_getmetatable(l, "SpriteMetaTable");
        lua_setmetatable(l, -2);
        return 1;
    }

    return 0;
}

int LuaAPI::lua_get_transform_field(lua_State* l)
{
    Transform* tr = (Transform*)lua_touserdata(l, -2);
    const char* field = lua_tostring(l, -1);

    lua_getglobal(l, "Transform");
    lua_pushstring(l, field);
    lua_rawget(l, -2);

    return 1;
}

int LuaAPI::lua_get_transform_translation(lua_State* l)
{
    Transform* tr = (Transform*)lua_touserdata(l, -1);

    create_vector3(l, tr->get_translation().x, tr->get_translation().y, tr->get_translation().z);

    return 1;
}

int LuaAPI::lua_set_transform_translation(lua_State* l)
{
    if (lua_isnumber(l, -1))
    {
        Transform* tr = (Transform*)lua_touserdata(l, -4);

        float x = lua_tonumber(l, -3);
        float y = lua_tonumber(l, -2);
        float z = lua_tonumber(l, -1);

        tr->set_translation(Vector3f(x, y, z));
    }
    else if (lua_istable(l, -1))
    {
        Transform* tr = (Transform*)lua_touserdata(l, -2);

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
        Transform* tr = (Transform*)lua_touserdata(l, -4);

        float x = lua_tonumber(l, -3);
        float y = lua_tonumber(l, -2);
        float z = lua_tonumber(l, -1);

        tr->set_translation(tr->get_translation() + Vector3f(x, y, z));
    }
    else if (lua_istable(l, -1))
    {
        Transform* tr = (Transform*)lua_touserdata(l, -2);

        float x = get_vector3_num(l, "x", -2);
        float y = get_vector3_num(l, "y", -2);
        float z = get_vector3_num(l, "z", -2);

        tr->set_translation(tr->get_translation() + Vector3f(x, y, z));
    }

    return 0;
}

int LuaAPI::lua_get_transform_rotation(lua_State* l)
{
    Transform* tr = (Transform*)lua_touserdata(l, -1);

    create_vector3(l, tr->get_rotation().x, tr->get_rotation().y, tr->get_rotation().z);

    return 1;
}

int LuaAPI::lua_set_transform_rotation(lua_State* l)
{
    if (lua_isnumber(l, -1))
    {
        Transform* tr = (Transform*)lua_touserdata(l, -4);

        float x = lua_tonumber(l, -3);
        float y = lua_tonumber(l, -2);
        float z = lua_tonumber(l, -1);

        tr->set_rotation(Vector3f(x, y, z));
    }
    else if (lua_istable(l, -1))
    {
        Transform* tr = (Transform*)lua_touserdata(l, -2);

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
        Transform* tr = (Transform*)lua_touserdata(l, -4);

        float x = lua_tonumber(l, -3);
        float y = lua_tonumber(l, -2);
        float z = lua_tonumber(l, -1);

        tr->set_rotation(tr->get_rotation() + Vector3f(x, y, z));
    }
    else if (lua_istable(l, -1))
    {
        Transform* tr = (Transform*)lua_touserdata(l, -2);

        float x = get_vector3_num(l, "x", -2);
        float y = get_vector3_num(l, "y", -2);
        float z = get_vector3_num(l, "z", -2);

        tr->set_rotation(tr->get_rotation() + Vector3f(x, y, z));
    }

    return 0;
}

int LuaAPI::lua_get_transform_scale(lua_State* l)
{
    Transform* tr = (Transform*)lua_touserdata(l, -1);

    create_vector3(l, tr->get_scale().x, tr->get_scale().y, tr->get_scale().z);

    return 1;
}

int LuaAPI::lua_set_transform_scale(lua_State* l)
{
    if (lua_isnumber(l, -1))
    {
        Transform* tr = (Transform*)lua_touserdata(l, -4);

        float x = lua_tonumber(l, -3);
        float y = lua_tonumber(l, -2);
        float z = lua_tonumber(l, -1);

        tr->set_scale(Vector3f(x, y, z));
    }
    else if (lua_istable(l, -1))
    {
        Transform* tr = (Transform*)lua_touserdata(l, -2);

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
        Transform* tr = (Transform*)lua_touserdata(l, -4);

        float x = lua_tonumber(l, -3);
        float y = lua_tonumber(l, -2);
        float z = lua_tonumber(l, -1);

        tr->set_scale(tr->get_scale() * Vector3f(x, y, z));
    }
    else if (lua_istable(l, -1))
    {
        Transform* tr = (Transform*)lua_touserdata(l, -2);

        float x = get_vector3_num(l, "x", -2);
        float y = get_vector3_num(l, "y", -2);
        float z = get_vector3_num(l, "z", -2);

        tr->set_scale(tr->get_scale() * Vector3f(x, y, z));
    }

    return 0;
}

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

int LuaAPI::lua_get_camera_field(lua_State* l)
{
    const char* field = lua_tostring(l, -1);

    if (!strcmp(field, "main"))
    {
        lua_pushlightuserdata(l, (void*)find_main_camera());

        luaL_getmetatable(l, "CameraMetaTable");
        lua_setmetatable(l, -2);

        return 1;
    }

    lua_getglobal(l, "Camera");
    lua_pushstring(l, field);
    lua_rawget(l, -2);

    return 1;
}

int LuaAPI::lua_get_audio_source_field(lua_State* l)
{
    const char* field = lua_tostring(l, -1);

    lua_getglobal(l, "AudioSource");
    lua_pushstring(l, field);
    lua_rawget(l, -2);

    return 1;
}

int LuaAPI::lua_audio_source_play(lua_State* l)
{
    AudioSource* audio = (AudioSource*)lua_touserdata(l, -1);
    audio->play();
    return 0;
}

int LuaAPI::lua_audio_source_pause(lua_State* l)
{
    AudioSource* audio = (AudioSource*)lua_touserdata(l, -1);
    audio->pause();
    return 0;
}

int LuaAPI::lua_get_sprite_field(lua_State* l)
{
    const char* field = lua_tostring(l, -1);

    lua_getglobal(l, "Sprite");
    lua_pushstring(l, field);
    lua_rawget(l, -2);

    return 1;
}

int LuaAPI::lua_set_sprite_color(lua_State* l)
{
    Sprite* sprite = (Sprite*)lua_touserdata(l, -2);

    float r = get_vector3_num(l, "x", -2);
    float g = get_vector3_num(l, "y", -2);
    float b = get_vector3_num(l, "z", -2);

    sprite->set_color(Vector3f(r, g, b));

    return 0;
}

void LuaAPI::register_api(const LuaScript& script)
{
    lua_State* l = script.get_lua_state();

    lua_newtable(l);
    lua_pushcfunction(l, lua_is_mouse_pressed);
    lua_setfield(l, -2, "is_mouse_pressed");
    lua_pushcfunction(l, lua_get_mouse_position);
    lua_setfield(l, -2, "get_mouse_position");
    lua_setglobal(l, "Mouse");

    lua_newtable(l);
    lua_pushcfunction(l, lua_is_key_pressed);
    lua_setfield(l, -2, "is_key_pressed");
    lua_setglobal(l, "Keyboard");

    lua_newtable(l);
    lua_pushcfunction(l, lua_get_this);
    lua_setfield(l, -2, "this");
    lua_pushcfunction(l, lua_get_component);
    lua_setfield(l, -2, "get_component");
    lua_pushcfunction(l, lua_has_component);
    lua_setfield(l, -2, "has_component");
    lua_pushcfunction(l, lua_create_component);
    lua_setfield(l, -2, "create_component");
    lua_pushcfunction(l, lua_gameobject_create);
    lua_setfield(l, -2, "create");
    lua_pushcfunction(l, lua_gameobject_delete);
    lua_setfield(l, -2, "delete");
    lua_pushcfunction(l, lua_gameobject_create_child);
    lua_setfield(l, -2, "create_child");
    lua_pushcfunction(l, lua_gameobject_get_child);
    lua_setfield(l, -2, "get_child");
    lua_setglobal(l, "GameObject");

    luaL_newmetatable(l, "GameObjectMetaTable");
    lua_pushstring(l, "__index");
    lua_pushcfunction(l, lua_get_gameobject_field);
    lua_settable(l, -3);

    lua_newtable(l);

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

    lua_setglobal(l, "Transform");

    luaL_newmetatable(l, "TransformMetaTable");
    lua_pushstring(l, "__index");
    lua_pushcfunction(l, lua_get_transform_field);
    lua_settable(l, -3);

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

    lua_newtable(l);
    lua_setglobal(l, "Camera");

    luaL_newmetatable(l, "CameraMetaTable");
    lua_pushstring(l, "__index");
    lua_pushcfunction(l, lua_get_camera_field);
    lua_settable(l, -3);

    lua_newtable(l);
    lua_pushcfunction(l, lua_audio_source_play);
    lua_setfield(l, -2, "play");
    lua_pushcfunction(l, lua_audio_source_pause);
    lua_setfield(l, -2, "pause");
    lua_setglobal(l, "AudioSource");

    luaL_newmetatable(l, "AudioSourceMetaTable");
    lua_pushstring(l, "__index");
    lua_pushcfunction(l, lua_get_audio_source_field);
    lua_settable(l, -3);

    lua_newtable(l);
    lua_pushcfunction(l, lua_set_sprite_color);
    lua_setfield(l, -2, "set_color");
    lua_setglobal(l, "Sprite");

    luaL_newmetatable(l, "SpriteMetaTable");
    lua_pushstring(l, "__index");
    lua_pushcfunction(l, lua_get_sprite_field);
    lua_settable(l, -3);

    lua_newtable(l);
    lua_pushstring(l, "dumpstack");
    lua_pushcfunction(l, lua_dumpstack);
    lua_settable(l, -3);
    lua_setglobal(l, "debug");
}