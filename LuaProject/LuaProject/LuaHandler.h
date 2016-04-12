#ifndef LUAHANDLER_H
#define LUAHANDLER_H

#include "lua.hpp"
#include "Unit.h"

class LuaHandler
{
private:
	lua_State* L;
public:
	LuaHandler();
	~LuaHandler();

	void start();
};

#endif