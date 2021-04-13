#pragma once

#include <string>
#include <vector>
#include <unordered_map>

#include "engine/scene/component.h"

struct lua_State;

class LuaGlobalVar
{
public:
    enum class Type
    {
        Number, String, Boolean, Userdata
    };

public:
    LuaGlobalVar(const std::string& name_)
        : name(name_) {}

    Type type = Type::Number;

    std::string name = "";
    
    float number;
    std::string string;
    bool boolean;
    void* userdata;
};

class LuaScript : public Component
{
public:
    LuaScript();
    LuaScript(const std::string& script);
    ~LuaScript();

    void on_start() override;
    void on_update(float dt) override;
    void on_destroy() override;

    void load_script(const std::string& script);

    lua_State* get_lua_state() const { return m_lua; }

    const std::string& get_path() const { return m_path; }

    static inline std::vector<LuaScript*> scripts;

    void serialize(YAML::Node& node) override;

    std::vector<LuaGlobalVar> global_vars;

private:
    lua_State* m_lua;
    std::string m_path = "";
};