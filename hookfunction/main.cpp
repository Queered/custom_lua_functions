#include <lua.hpp>
#include <iostream>

static int my_hook(lua_State* L)
{
    const char* function_name = lua_tostring(L, lua_upvalueindex(1));
    std::cout << "Hooked function: " << function_name << std::endl; // debug
    return lua_gettop(L);
}

static int hookfunction(lua_State* L)
{
    luaL_checktype(L, 1, LUA_TFUNCTION);
    luaL_checktype(L, 2, LUA_TSTRING);

    const char* function_name = lua_tostring(L, 2);

    lua_pushstring(L, function_name);
    lua_pushcclosure(L, my_hook, 1);

    lua_pushvalue(L, 1);
    lua_pushvalue(L, -2);
    lua_settable(L, LUA_REGISTRYINDEX);

    lua_pop(L, 1);

    return 0;
}

int main()
{
    lua_State* L = luaL_newstate();
    luaL_openlibs(L);

    lua_pushcfunction(L, hookfunction);
    lua_setglobal(L, "hookfunction");

    luaL_dostring(L, "function my_function()\n    print('Hello, world!')\nend\nhookfunction(my_function, 'my_function')\nmy_function()"); // test script (biggest bullshit on the world)

    lua_close(L);
    return 0;
}
