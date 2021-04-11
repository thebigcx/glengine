#pragma once

#include <string>

struct lua_State;

class LuaScript;
class Node;
class Camera;

class LuaAPI
{
public:
    static int lua_dumpstack(lua_State* l);

    static Node* find_node(lua_State* l);
    static Camera* find_main_camera();

    static int lua_vector3_new(lua_State* l);
    static int lua_vector3_add(lua_State* l);
    static int lua_vector3_sub(lua_State* l);
    static int lua_vector3_mul(lua_State* l);
    static int lua_vector3_div(lua_State* l);

    static int lua_get_this(lua_State* l);
    static int lua_get_gameobject_field(lua_State* l);
    static int lua_get_component(lua_State* l);
    static int lua_has_component(lua_State* l);
    static int lua_find_gameobject_name(lua_State* l);
    static int lua_gameobject_create(lua_State* l);
    static int lua_gameobject_delete(lua_State* l);
    static int lua_gameobject_get_child(lua_State* l);
    static int lua_gameobject_create_child(lua_State* l);
    static int lua_create_component(lua_State* l);

    static int lua_get_transform_field(lua_State* l);

    static int lua_get_transform_translation(lua_State* l);
    static int lua_set_transform_translation(lua_State* l);
    static int lua_transform_translate(lua_State* l);

    static int lua_get_transform_rotation(lua_State* l);
    static int lua_set_transform_rotation(lua_State* l);
    static int lua_transform_rotate(lua_State* l);

    static int lua_get_transform_scale(lua_State* l);
    static int lua_set_transform_scale(lua_State* l);
    static int lua_transform_scale(lua_State* l);

    static float get_vector3_num(lua_State* l, const std::string& field, int table);
    static void create_vector3(lua_State* l, float x, float y, float z);

    static int lua_is_mouse_pressed(lua_State* l);
    static int lua_get_mouse_position(lua_State* l);
    static int lua_is_key_pressed(lua_State* l);

    static int lua_get_camera_field(lua_State* l);

    static int lua_get_audio_source_field(lua_State* l);
    static int lua_audio_source_play(lua_State* l);
    static int lua_audio_source_pause(lua_State* l);

    static int lua_get_sprite_field(lua_State* l);
    static int lua_set_sprite_color(lua_State* l);

    static void register_api(const LuaScript& script);
};