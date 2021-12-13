#include <lua/lua.hpp>
#include "Common.h"

class LuaContext
{
public:

    LuaContext();
    ~LuaContext();

    inline lua_State* State() { return L_; }

    void Register(const char* name, lua_CFunction f);
    int DoFile(const char* filename);

    int GetGlobal(const char* name);
    int IsFunction(int index);
    void PushNumber(lua_Number n);
    void Call(int nargs, int nresults);

private:
    lua_State* L_;

};
