#ifndef LUAHANDLER_H
#define LUAHANDLER_H

#include "Player.h"

class LuaHandler
{
private:
	lua_State* L;
	sf::RenderTarget* targetPtr;

public:
	LuaHandler();
	LuaHandler(sf::RenderTarget* target);
	~LuaHandler();

	void start(const char* scriptName);
	void load(const char* scriptName);
};

#endif