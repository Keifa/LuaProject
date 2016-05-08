#pragma once
#include <iostream>
#include <string>
#include "lua.hpp"

class Player
{
private:
	std::string _name;
	int _x;
	int _y;

public:
	Player(const std::string name)
	{
		_name = name;
	}

	void Print()
	{
		std::cout << "[c++ class] My name is " << _name << "\n";
	}

	int Jump(int force)
	{
		return force * 2;
	}

	void SetPosition(int x, int y)
	{
		_x = x;
		_y = y;
	}

	int GetX()
	{
		return _x;
	}

	int GetY()
	{
		return _y;
	}
};

int player_create(lua_State* ls)
{
	const char* name = lua_tolstring(ls, 1, nullptr);
	if (name != nullptr)
	{
		Player** player = reinterpret_cast<Player**>(lua_newuserdata(ls, sizeof(Player*)));
		*player = new Player(name);

		luaL_getmetatable(ls, "MetaPlayer");
		lua_setmetatable(ls, -2);

		std::cout << "[c++] Created player\n";
	}
	else {
		//something is fucked up
	}
	return 1;
}

Player* l_CheckPlayer(lua_State* ls, int n)
{
	Player* playerPtr = nullptr;

	void* ptr = luaL_testudata(ls, n, "MetaPlayer");
	if (ptr != nullptr)
	{
		playerPtr = *(Player**)ptr;
	}

	return playerPtr;
}

int player_destroy(lua_State* ls)
{
	Player* player = l_CheckPlayer(ls, 1);
	delete player;

	std::cout << "deleted player\n";

	return 0;
}

int player_print(lua_State* ls)
{
	Player* player = l_CheckPlayer(ls, 1);
	player->Print();

	return 0;
}

int player_jump(lua_State* ls)
{
	Player* player = l_CheckPlayer(ls, 1);

	int isnum = 0;
	int force = (int)lua_tonumberx(ls, 2, &isnum);

	int result = -1;
	if (isnum != 0)
	{
		result = player->Jump(force);
	}
	lua_pushinteger(ls, result);
	return 1;
}

int player_getX(lua_State* ls)
{
	Player* player = l_CheckPlayer(ls, 1);

	lua_pushinteger(ls, player->GetX());
	return 1;
}

int player_getY(lua_State* ls)
{
	Player* player = l_CheckPlayer(ls, 1);

	lua_pushinteger(ls, player->GetY());
	return 1;
}

int player_setPosition(lua_State* ls)
{
	Player* player = l_CheckPlayer(ls, 1);
	player->SetPosition(lua_tointeger(ls, 2), lua_tointeger(ls, 3));
	return 0;
}

void RegisterPlayer(lua_State* ls)
{
	luaL_newmetatable(ls, "MetaPlayer");

	luaL_Reg sPlayerRegs[] =
	{
		{"New",			player_create},
		{"Print",		player_print},
		{"Jump",		player_jump},
		{"SetPosition",	player_setPosition},
		{"GetX",		player_getX},
		{"GetY",		player_getY},
		{"__gc",		player_destroy},
		{NULL, NULL}
	};

	luaL_setfuncs(ls, sPlayerRegs, 0);

	lua_pushvalue(ls, -1);

	lua_setfield(ls, -1, "__index");

	lua_setglobal(ls, "Player");
}