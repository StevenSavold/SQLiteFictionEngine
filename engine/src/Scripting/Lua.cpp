#include "Lua.h"

LuaContext::LuaContext()
    : L_(luaL_newstate())
{
    /* Open the lua std libs for use */
    luaL_openlibs(L_);
}

LuaContext::~LuaContext()
{
    lua_close(L_);
}


void LuaContext::Register(const char* name, lua_CFunction f)
{
    lua_register(L_, name, f);
}

int LuaContext::DoFile(const char* filename)
{
    return luaL_dofile(L_, filename);
}

int LuaContext::GetGlobal(const char* name)
{
    return lua_getglobal(L_, name);
}

int LuaContext::IsFunction(int index)
{
    return lua_isfunction(L_, index);
}

void LuaContext::PushNumber(lua_Number n)
{
    lua_pushnumber(L_, n);
}

void LuaContext::Call(int nargs, int nresults)
{
    lua_call(L_, nargs, nresults);
}
