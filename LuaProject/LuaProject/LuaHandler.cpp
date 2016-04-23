#include "LuaHandler.h"

int createPlayer(lua_State * ls);
Player* l_CheckPlayer(lua_State * ls, int n);
int destroyPlayer(lua_State * ls);
int printPlayer(lua_State * ls);
int setPositionPlayer(lua_State* ls);
int movePlayer(lua_State* ls);
int drawPlayer(lua_State* ls);
void registerPlayer(lua_State * ls);

LuaHandler::LuaHandler()
{
	
}

LuaHandler::LuaHandler(sf::RenderTarget* target)
{
	targetPtr = target;
}

LuaHandler::~LuaHandler()
{
	lua_close(L);
}

void LuaHandler::start(const char* scriptName)
{
	std::cout << "[C++] Starting Lua state\n";
	this->L = luaL_newstate();

	if (this->L)
	{
		// Load Lua libraries
		std::cout << "[C++] Loading Lua libraries\n";
		luaL_openlibs(L);

		// Register UnitClass
		registerPlayer(L);

		// Load the script
		std::cout << "[C++] Loading the Lua script\n";
		int error = luaL_loadfile(L, scriptName)
			|| lua_pcall(L, 0, 0, 0);

		if (error)
		{
			std::cerr << lua_tostring(L, -1) << std::endl;
			lua_pop(L, 1);
		}

		// close the Lua state
		std::cout << "[C++] Closing the Lua state\n";
		
	}
}

void LuaHandler::load(const char * scriptName)
{
	int error = luaL_loadfile(L, scriptName)
		|| lua_pcall(L, 0, 0, 0);

	if (error)
	{
		std::cerr << lua_tostring(L, -1) << std::endl;
		lua_pop(L, 1);
	}
}

int createPlayer(lua_State * ls)
{
	const char* name = lua_tolstring(ls, 1, nullptr);
	if (name != nullptr)
	{
		Player** player = reinterpret_cast<Player**>(lua_newuserdata(ls, sizeof(Player*)));
		*player = new Player(name);

		luaL_getmetatable(ls, "MetaPlayer");
		lua_setmetatable(ls, -2);

		std::cout << "[C++] A Unit was created\n";
	}
	else
		std::cout << "[C++] A Unit unsuccessfully created\n";

	return 1;
}

Player* l_CheckPlayer(lua_State * ls, int n)
{
	Player* playerPtr = nullptr;
	void* ptr = luaL_testudata(ls, n, "MetaPlayer");
	if (ptr != nullptr)
		playerPtr = *(Player**)ptr;
	return playerPtr;
}

int destroyPlayer(lua_State * ls)
{
	Player* player = l_CheckPlayer(ls, 1);
	delete player;
	return 0;
}

int printPlayer(lua_State * ls)
{
	Player* player = l_CheckPlayer(ls, 1);
	player->print();
	return 0;
}

int setPositionPlayer(lua_State* ls)
{
	Player* player = l_CheckPlayer(ls, 1);
	player->setPosition(lua_tointeger(ls, 2), lua_tointeger(ls, 3));
	return 0;
}

int movePlayer(lua_State* ls)
{
	Player* player = l_CheckPlayer(ls, 1);
	player->move(lua_tointeger(ls, 2), lua_tointeger(ls, 3));
	return 0;
}

int drawPlayer(lua_State* ls)
{
	Player* player = l_CheckPlayer(ls, 1);
	player->draw();
	return 0;
}

void registerPlayer(lua_State * ls)
{
	// Create a luaL metatable. This metatable is not 
	// exposed to Lua. The "luaL_Foo" label is used by luaL
	// internally to identity things.
	luaL_newmetatable(ls, "MetaPlayer");
	// The Lua stack at this point looks like this:
	//     
	//     1| metatable "MetaMonster"   |-1

	luaL_Reg playerRegs[] =
	{
		{ "New",			createPlayer },
		{ "Print",			printPlayer },
		{ "SetPosition",	setPositionPlayer},
		{ "Move",			movePlayer},
		{ "Draw",			drawPlayer},
		{ "__gc",			destroyPlayer },
		{ NULL, NULL }
	};

	// Register the C functions _into_ the metatable we just created.
	luaL_setfuncs(ls, playerRegs, 0);
	lua_pushvalue(ls, -1);

	// The Lua stack at this point looks like this:
	//     
	//     2| metatable "MetaMonster"   |-1
	//     1| metatable "MetaMonster"   |-2

	// Set the "__index" field of the metatable to point to itself
	// This pops the stack
	lua_setfield(ls, -1, "__index");

	// The Lua stack at this point looks like this:
	//     
	//     1| metatable "MetaMonster"   |-1

	// The luaL_Foo metatable now has the following fields
	//     - __gc
	//     - __index
	//     - add
	//     - print
	//     - new

	// Now we use setglobal to officially expose the luaL_Foo metatable 
	// to Lua. And we use the name "Monster".
	//
	// This allows Lua scripts to _override_ the metatable of Monster.
	// For high security code this may not be called for but 
	// we'll do this to get greater flexibility.
	lua_setglobal(ls, "Player");
}