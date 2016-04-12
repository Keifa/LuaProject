#include <iostream>
#include "lua.hpp"

int main()
{
	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);

	lua_State* L =  luaL_newstate();

	std::system("pause");
	return 0;
}