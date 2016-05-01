#pragma once
#include "lua.hpp"

void PrintTable(lua_State* L)
{
	for (lua_pushnil(L); lua_next(L, -2); lua_pop(L, 1))
	{
		if (lua_isstring(L, -1))
			printf("%s = %s\n", lua_tostring(L, -2), lua_tostring(L, -1));
		else if (lua_isnumber(L, -1))
			printf("%s = %d\n", lua_tostring(L, -2), lua_tonumber(L, -1));
		else if (lua_istable(L, -1))
			PrintTable(L);
	}
}