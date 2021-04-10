#pragma once

#include <string>
#include <vector>

#include "engine/component.h"

struct lua_State;

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

    static inline std::vector<LuaScript*> scripts;

private:
    lua_State* m_lua;
    std::string m_path = "";
};