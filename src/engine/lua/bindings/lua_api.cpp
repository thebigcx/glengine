#include "engine/lua/bindings/lua_api.h"
#include "engine/lua/lua_script.h"
#include "engine/lua/lua.h"
#include "engine/scene/node.h"
#include "engine/core/mouse.h"
#include "engine/core/keyboard.h"
#include "engine/renderer/camera.h"
#include "engine/scene/scene.h"
#include "engine/audio/audio.h"
#include "engine/renderer/sprite.h"
#include "engine/renderer/assets.h"
#include "engine/renderer/mesh.h"
#include "engine/renderer/mesh_renderer.h"
#include "engine/core/deserializer.h"

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

int LuaAPI::lua_get_camera_field(lua_State* l)
{
    const char* field = lua_tostring(l, -1);

    lua_getglobal(l, "Camera");
    lua_pushstring(l, field);
    lua_rawget(l, -2);

    return 1;
}

int LuaAPI::lua_get_main_camera(lua_State* l)
{
    Camera** ptr = (Camera**)lua_newuserdata(l, 8);
    *ptr = find_main_camera();

    luaL_getmetatable(l, "CameraMetaTable");
    lua_setmetatable(l, -2);

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
    AudioSource* audio = *(AudioSource**)lua_touserdata(l, -1);
    audio->play();
    return 0;
}

int LuaAPI::lua_audio_source_pause(lua_State* l)
{
    AudioSource* audio = *(AudioSource**)lua_touserdata(l, -1);
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
    Sprite* sprite = *(Sprite**)lua_touserdata(l, -2);

    float r = get_vector3_num(l, "x", -2);
    float g = get_vector3_num(l, "y", -2);
    float b = get_vector3_num(l, "z", -2);

    sprite->set_color(Vector3f(r, g, b));

    return 0;
}

int LuaAPI::lua_set_sprite_texture(lua_State* l)
{
    Sprite* sprite = *(Sprite**)lua_touserdata(l, -2);

    const char* path = lua_tostring(l, -1);

    sprite->set_texture(AssetManager::get_instance()->get_texture(path));

    return 0;
}

int LuaAPI::lua_sprite_use_uvs(lua_State* l)
{
    Sprite* sprite = *(Sprite**)lua_touserdata(l, -2);

    bool use_uvs = lua_toboolean(l, -1);
    sprite->use_uvs(use_uvs);

    return 0;
}

int LuaAPI::lua_set_sprite_uv1(lua_State* l)
{
    Sprite* sprite = *(Sprite**)lua_touserdata(l, -3);

    float x = lua_tonumber(l, -2);
    float y = lua_tonumber(l, -1);
    sprite->set_uv1(Vector2f(x, y));

    return 0;
}

int LuaAPI::lua_set_sprite_uv2(lua_State* l)
{
    Sprite* sprite = *(Sprite**)lua_touserdata(l, -3);

    float x = lua_tonumber(l, -2);
    float y = lua_tonumber(l, -1);
    sprite->set_uv2(Vector2f(x, y));

    return 0;
}

int LuaAPI::lua_get_mesh_field(lua_State* l)
{
    const char* field = lua_tostring(l, -1);

    lua_getglobal(l, "Mesh");
    lua_pushstring(l, field);
    lua_rawget(l, -2);

    return 1;
}

int LuaAPI::lua_get_mesh_renderer_field(lua_State* l)
{
    const char* field = lua_tostring(l, -1);

    lua_getglobal(l, "MeshRenderer");
    lua_pushstring(l, field);
    lua_rawget(l, -2);

    return 1;
}

int LuaAPI::lua_mesh_set_mesh(lua_State* l)
{
    Mesh* mesh = *(Mesh**)lua_touserdata(l, -2);
    const char* path = lua_tostring(l, -1);
    mesh->load(path);
    return 0;
}

int LuaAPI::lua_mesh_renderer_set_material(lua_State* l)
{
    MeshRenderer* mesh_renderer = *(MeshRenderer**)lua_touserdata(l, -2);
    const char* material = lua_tostring(l, -1);
    mesh_renderer->set_material(AssetManager::get_instance()->get_material(material));

    return 0;
}

// TODO: fix this function
int LuaAPI::lua_switch_scene(lua_State* l)
{
    const char* path = lua_tostring(l, -1);

    //Scene::current_scene = Deserializer::deserialize_scene(path);
    //Scene::current_scene->on_start();
    return 0;
}

int LuaAPI::lua_get_scene_field(lua_State* l)
{
    const char* field = lua_tostring(l, -1);

    lua_getglobal(l, "Scene");
    lua_pushstring(l, field);
    lua_rawget(l, -2);

    return 1;
}

int LuaAPI::lua_scene_get_root_node(lua_State* l)
{
    Scene* scene = *(Scene**)lua_touserdata(l, -1);
    Node* node = scene->get_root_node();

    Node** ptr = (Node**)lua_newuserdata(l, 8);
    *ptr = node;

    luaL_getmetatable(l, "GameObjectMetaTable");
    lua_setmetatable(l, -2);

    return 1;
}

int LuaAPI::lua_scene_get_current(lua_State* l)
{
    Scene* scene = Scene::current_scene;
    Scene** ptr = (Scene**)lua_newuserdata(l, 8);
    *ptr = scene;

    luaL_getmetatable(l, "SceneMetaTable");
    lua_setmetatable(l, -2);

    return 1;
}

void LuaAPI::register_api(const LuaScript& script)
{
    lua_State* l = script.get_lua_state();

    register_gameobject_functions(l);
    register_input_functions(l);
    register_transform_functions(l);

    lua_getglobal(l, "require");
    lua_pushstring(l, "src/engine/lua/bindings/native/vector3");
    lua_call(l, 1, 1);

    lua_newtable(l);
    lua_pushvalue(l, lua_gettop(l));
    lua_setglobal(l, "Camera");
    lua_pushcfunction(l, lua_get_component_owner);
    lua_setfield(l, -2, "get_owner");
    lua_pushcfunction(l, lua_get_main_camera);
    lua_setfield(l, -2, "get_main");

    luaL_newmetatable(l, "CameraMetaTable");
    lua_pushstring(l, "__index");
    lua_pushcfunction(l, lua_get_camera_field);
    lua_settable(l, -3);

    lua_newtable(l);
    lua_pushvalue(l, lua_gettop(l));
    lua_setglobal(l, "AudioSource");
    lua_pushcfunction(l, lua_audio_source_play);
    lua_setfield(l, -2, "play");
    lua_pushcfunction(l, lua_audio_source_pause);
    lua_setfield(l, -2, "pause");

    luaL_newmetatable(l, "AudioSourceMetaTable");
    lua_pushstring(l, "__index");
    lua_pushcfunction(l, lua_get_audio_source_field);
    lua_settable(l, -3);

    lua_newtable(l);
    lua_pushvalue(l, lua_gettop(l));
    lua_setglobal(l, "Sprite");
    lua_pushcfunction(l, lua_set_sprite_color);
    lua_setfield(l, -2, "set_color");
    lua_pushcfunction(l, lua_set_sprite_texture);
    lua_setfield(l, -2, "set_texture");
    lua_pushcfunction(l, lua_sprite_use_uvs);
    lua_setfield(l, -2, "use_uvs");
    lua_pushcfunction(l, lua_set_sprite_uv1);
    lua_setfield(l, -2, "set_uv1");
    lua_pushcfunction(l, lua_set_sprite_uv2);
    lua_setfield(l, -2, "set_uv2");

    luaL_newmetatable(l, "SpriteMetaTable");
    lua_pushstring(l, "__index");
    lua_pushcfunction(l, lua_get_sprite_field);
    lua_settable(l, -3);

    lua_newtable(l);
    lua_pushvalue(l, lua_gettop(l));
    lua_pushcfunction(l, lua_mesh_set_mesh);
    lua_setfield(l, -2, "set_mesh");
    lua_setglobal(l, "Mesh");

    luaL_newmetatable(l, "MeshMetaTable");
    lua_pushstring(l, "__index");
    lua_pushcfunction(l, lua_get_mesh_field);
    lua_settable(l, -3);

    lua_newtable(l);
    lua_pushvalue(l, lua_gettop(l));
    lua_pushcfunction(l, lua_mesh_renderer_set_material);
    lua_setfield(l, -2, "set_material");
    lua_setglobal(l, "MeshRenderer");

    luaL_newmetatable(l, "MeshRendererMetaTable");
    lua_pushstring(l, "__index");
    lua_pushcfunction(l, lua_get_mesh_renderer_field);
    lua_settable(l, -3);

    lua_newtable(l);
    lua_pushvalue(l, lua_gettop(l));
    lua_setglobal(l, "Scene");
    lua_pushcfunction(l, lua_scene_get_current);
    lua_setfield(l, -2, "get_current");
    lua_pushcfunction(l, lua_switch_scene);
    lua_setfield(l, -2, "switch_scene");
    lua_pushcfunction(l, lua_scene_get_root_node);
    lua_setfield(l, -2, "get_root_gameobject");

    luaL_newmetatable(l, "SceneMetaTable");
    lua_pushstring(l, "__index");
    lua_pushcfunction(l, lua_get_scene_field);
    lua_settable(l, -3);

    lua_newtable(l);
    lua_pushstring(l, "dumpstack");
    lua_pushcfunction(l, lua_dumpstack);
    lua_settable(l, -3);
    lua_setglobal(l, "debug");
}