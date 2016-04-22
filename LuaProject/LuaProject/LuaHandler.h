#ifndef LUAHANDLER_H
#define LUAHANDLER_H

#include "Player.h"

class LuaHandler
{
private:
	lua_State* L;
public:
	LuaHandler();
	~LuaHandler();

	void start(const char* scriptName);
	void load(const char* scriptName);
};

#endif