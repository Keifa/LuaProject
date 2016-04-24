#include "LuaScript.h"


LuaScript::LuaScript(const std::string & filename)
{
	L = luaL_newstate();
	luaL_openlibs(L);
	if (luaL_loadfile(L, filename.c_str()) || lua_pcall(L,0,0,0))
	{
		std::cout << "Error: script not loaded (" << filename << ")" << std::endl;
		L = 0;
	}
}

LuaScript::~LuaScript()
{
	if (L) lua_close(L);
}

void LuaScript::printError(const std::string & variableName, const std::string reason)
{
	std::cout << "Error: can't get [" << variableName << "]. " << reason << std::endl;
}

std::vector<int> LuaScript::getIntVector(const std::string & variableName)
{
	std::vector<int> v;
	lua_getglobal(L, variableName.c_str());
	if (lua_isnil(L,-1))
	{
		return std::vector<int>();
	}
	for (lua_pushnil(L); lua_next(L, -2); lua_pop(L, 1))
	{
		v.push_back((int)lua_tonumber(L, -1));
	}
	clean();
	return v;
}
