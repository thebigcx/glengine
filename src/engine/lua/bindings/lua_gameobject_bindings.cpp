#include "engine/lua/bindings/lua_api.h"
#include "engine/renderer/transform.h"
#include "engine/renderer/sprite.h"
#include "engine/renderer/mesh.h"
#include "engine/renderer/mesh_renderer.h"
#include "engine/audio/audio.h"
#include "engine/scene/node.h"
#include "engine/renderer/camera.h"
#include "engine/lua/lua_script.h"

#include "engine/lua/lua.h"

int LuaAPI::lua_get_this(lua_State* l)
{
    Node* node = find_node(l);
    Node** ptr = (Node**)lua_newuserdata(l, 8);
    *ptr = node;

    luaL_getmetatable(l, "GameObjectMetaTable");
    lua_setmetatable(l, -2);

    return 1;
}

int LuaAPI::lua_get_gameobject_field(lua_State* l)
{
    Node* node = *(Node**)lua_touserdata(l, -2);
    const char* field = lua_tostring(l, -1);

    if (!strcmp(field, "transform"))
    {
        Transform** tr = (Transform**)lua_newuserdata(l, 8); // Assumption of pointer size 8 bytes
        *tr = &node->get_transform();

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
    Node* node = *(Node**)lua_touserdata(l, -2);
    const char* component = lua_tostring(l, -1);

    if (!strcmp(component, "Sprite"))
    {
        Sprite* com = node->get_component<Sprite>();
        Sprite** ptr = (Sprite**)lua_newuserdata(l, 8);
        *ptr = com;
        luaL_getmetatable(l, "SpriteMetaTable");
        lua_setmetatable(l, -2);

        return 1;
    }
    else if (!strcmp(component, "AudioSource"))
    {
        AudioSource* com = node->get_component<AudioSource>();
        AudioSource** ptr = (AudioSource**)lua_newuserdata(l, 8);
        *ptr = com;
        luaL_getmetatable(l, "AudioSourceMetaTable");
        lua_setmetatable(l, -2);

        return 1;
    }
    else if (!strcmp(component, "Mesh"))
    {
        Mesh* com = node->get_component<Mesh>();
        Mesh** ptr = (Mesh**)lua_newuserdata(l, 8);
        *ptr = com;
        luaL_getmetatable(l, "MeshMetaTable");
        lua_setmetatable(l, -2);

        return 1;
    }
    else if (!strcmp(component, "MeshRenderer"))
    {
        MeshRenderer* com = node->get_component<MeshRenderer>();
        MeshRenderer** ptr = (MeshRenderer**)lua_newuserdata(l, 8);
        *ptr = com;
        luaL_getmetatable(l, "MeshRendererMetaTable");
        lua_setmetatable(l, -2);

        return 1;
    }
    else if (!strcmp(component, "Camera"))
    {
        Camera* com = node->get_component<Camera>();
        Camera** ptr = (Camera**)lua_newuserdata(l, 8);
        *ptr = com;
        luaL_getmetatable(l, "CameraMetaTable");
        lua_setmetatable(l, -2);

        return 1;
    }

    return 0;
}

int LuaAPI::lua_has_component(lua_State* l)
{
    Node* node = *(Node**)lua_touserdata(l, -2);
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
    Node* parent = *(Node**)lua_touserdata(l, -2);
    const char* child_name = lua_tostring(l, -1);

    Node** ptr = (Node**)lua_newuserdata(l, 8);
    *ptr = parent->find_child(child_name);
    
    luaL_getmetatable(l, "GameObjectMetaTable");
    lua_setmetatable(l, -2);

    return 1;
}

int LuaAPI::lua_gameobject_create_child(lua_State* l)
{
    Node* parent = *(Node**)lua_touserdata(l, -2);
    const char* child_name = lua_tostring(l, -1);

    Node* child = parent->create_child(child_name);

    Node** ptr = (Node**)lua_newuserdata(l, 8);
    *ptr = child;
    luaL_getmetatable(l, "GameObjectMetaTable");
    lua_setmetatable(l, -2);

    return 1;
}

int LuaAPI::lua_create_component(lua_State* l)
{
    Node* node = *(Node**)lua_touserdata(l, -2);
    const char* component = lua_tostring(l, -1);

    if (!strcmp(component, "Sprite"))
    {
        Sprite* sprite = node->create_component<Sprite>();
        sprite->on_start();
        Sprite** ptr = (Sprite**)lua_newuserdata(l, 8);
        *ptr = sprite;
        luaL_getmetatable(l, "SpriteMetaTable");
        lua_setmetatable(l, -2);
        return 1;
    }
    else if (!strcmp(component, "Camera"))
    {
        Camera* camera = node->create_component<Camera>();
        camera->on_start();
        Camera** ptr = (Camera**)lua_newuserdata(l, 8);
        *ptr = camera;
        luaL_getmetatable(l, "CameraMetaTable");
        lua_setmetatable(l, -2);
        return 1;
    }
    else if (!strcmp(component, "Mesh"))
    {
        Mesh* mesh = node->create_component<Mesh>();
        mesh->on_start();
        Mesh** ptr = (Mesh**)lua_newuserdata(l, 8);
        *ptr = mesh;
        luaL_getmetatable(l, "MeshMetaTable");
        lua_setmetatable(l, -2);
        return 1;
    }
    else if (!strcmp(component, "MeshRenderer"))
    {
        MeshRenderer* render = node->create_component<MeshRenderer>();
        render->on_start();
        MeshRenderer** ptr = (MeshRenderer**)lua_newuserdata(l, 8);
        *ptr = render;
        luaL_getmetatable(l, "MeshRendererMetaTable");
        lua_setmetatable(l, -2);
        return 1;
    }

    return 0;
}

int LuaAPI::lua_remove_component(lua_State* l)
{
    Node* node = *(Node**)lua_touserdata(l, -2);
    const char* component = lua_tostring(l, -1);

    if (!strcmp(component, "Sprite"))
    {
        Sprite* sprite = node->get_component<Sprite>();
        sprite->on_destroy();
        node->remove_component<Sprite>();
    }
    else if (!strcmp(component, "Camera"))
    {
        Camera* camera = node->get_component<Camera>();
        camera->on_destroy();
        node->remove_component<Camera>();
    }
    else if (!strcmp(component, "Mesh"))
    {
        Mesh* mesh = node->get_component<Mesh>();
        mesh->on_destroy();
        node->remove_component<Mesh>();
    }
    else if (!strcmp(component, "MeshRenderer"))
    {
        MeshRenderer* mesh_renderer = node->get_component<MeshRenderer>();
        mesh_renderer->on_destroy();
        node->remove_component<MeshRenderer>();
    }

    return 0;
}

int LuaAPI::lua_get_component_owner(lua_State* l)
{
    Component* component = *(Component**)lua_touserdata(l, -1);

    Node** owner = (Node**)lua_newuserdata(l, 8);
    *owner = component->get_owner();

    luaL_getmetatable(l, "GameObjectMetaTable");
    lua_setmetatable(l, -2);

    return 1;
}

void LuaAPI::register_gameobject_functions(lua_State* l)
{
    lua_newtable(l);
    lua_pushvalue(l, lua_gettop(l));
    lua_setglobal(l, "GameObject");
    lua_pushcfunction(l, lua_get_this);
    lua_setfield(l, -2, "this");
    lua_pushcfunction(l, lua_get_component);
    lua_setfield(l, -2, "get_component");
    lua_pushcfunction(l, lua_has_component);
    lua_setfield(l, -2, "has_component");
    lua_pushcfunction(l, lua_create_component);
    lua_setfield(l, -2, "create_component");
    lua_pushcfunction(l, lua_remove_component);
    lua_setfield(l, -2, "remove_component");
    lua_pushcfunction(l, lua_gameobject_create);
    lua_setfield(l, -2, "create");
    lua_pushcfunction(l, lua_gameobject_delete);
    lua_setfield(l, -2, "delete");
    lua_pushcfunction(l, lua_gameobject_create_child);
    lua_setfield(l, -2, "create_child");
    lua_pushcfunction(l, lua_gameobject_get_child);
    lua_setfield(l, -2, "get_child");

    luaL_newmetatable(l, "GameObjectMetaTable");
    lua_pushstring(l, "__index");
    lua_pushcfunction(l, lua_get_gameobject_field);
    lua_settable(l, -3);
}