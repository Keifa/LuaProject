#include "LuaHandler.h"

int createUnit(lua_State * ls);
Unit* l_CheckUnit(lua_State * ls, int n);
int destroyUnit(lua_State * ls);
void registerUnit(lua_State * ls);


LuaHandler::LuaHandler()
{
	
}

LuaHandler::~LuaHandler()
{
	
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
		registerUnit(L);

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
		lua_close(L);
	}
}

int createUnit(lua_State * ls)
{
	const char* name = lua_tolstring(ls, 1, nullptr);
	if(name != nullptr)
	{
		Unit** unit = reinterpret_cast<Unit**>(lua_newuserdata(ls, sizeof(Unit*)));
		*unit = new Unit(name);

		luaL_getmetatable(ls, "MetaUnit");
		lua_setmetatable(ls, -2);

		std::cout << "[C++] A Unit was created\n";
	}
	else
		std::cout << "[C++] A Unit unsuccessfully created\n";
	
	return 1;
}

Unit* l_CheckUnit(lua_State * ls, int n)
{
	Unit* unitPtr = nullptr;
	void* ptr = luaL_testudata(ls, n, "MetaUnit");
	if (ptr != nullptr)
		unitPtr = *(Unit**)ptr;
	return unitPtr;
}

int destroyUnit(lua_State * ls)
{
	Unit* unit = l_CheckUnit(ls, 1);
	delete unit;
	return 0;
}

int printUnit(lua_State * ls)
{
	Unit* unit = l_CheckUnit(ls, 1);
	unit->print();
	return 0;
}

void registerUnit(lua_State * ls)
{
	// Create a luaL metatable. This metatable is not 
	// exposed to Lua. The "luaL_Foo" label is used by luaL
	// internally to identity things.
	luaL_newmetatable(ls, "MetaUnit");
	// The Lua stack at this point looks like this:
	//     
	//     1| metatable "MetaMonster"   |-1

	luaL_Reg unitRegs[] =
	{
		{"New",		createUnit},
		{"Print",	printUnit},
		{"__gc",	destroyUnit},
		{NULL, NULL}
	};

	// Register the C functions _into_ the metatable we just created.
	luaL_setfuncs(ls, unitRegs, 0);
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
	lua_setglobal(ls, "Unit");
}
