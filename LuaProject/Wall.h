#pragma once
#include <iostream>
#include "lua.hpp"
#include "Util.h"

class Wall
{
private:
	int x, y;
	Colors color;
public:
	Wall(int x, int y)
	{
		this->x = x;
		this->y = y;
		color = BLACK;
	}

	void Print()
	{
		std::cout << "[C++ WALL] Position: " << x << ", " << y << std::endl;
	}

	void SetPosition(int x, int y)
	{
		this->x = x;
		this->y = y;
	}

	int GetX()
	{
		return x;
	}

	int GetY()
	{
		return y;
	}

	void SetColor(int color)
	{
		this->color = (Colors)color;
	}

	int GetColor()
	{
		return color;
	}

	bool CheckCollision(int x, int y)
	{
		bool check = false;
		if (this->x == x && this->y == y)
			check = true;
		return check;
	}
};

int WallCreate(lua_State* ls)
{	
	int isnum = 0, x = 0, y = 0;
	bool error = true;

	x = lua_tointegerx(ls, 1, &isnum);
	if (isnum != 0)
	{
		y = lua_tointegerx(ls, 2, &isnum);
		if (isnum != 0)
			error = false;
	}
	if (!error)
	{
		Wall** wall = reinterpret_cast<Wall**>(lua_newuserdata(ls, sizeof(Wall*)));
		*wall = new Wall(x, y);

		luaL_getmetatable(ls, "MetaWall");
		lua_setmetatable(ls, -2);

		std::cout << "[C++] Created Wall\n";
	}
	else
	{
		std::cout << "[C++] Failed to create Wall\n";
	}
	return 1;
}

Wall* l_CheckWall(lua_State* ls, int n)
{
	Wall* wallPtr = nullptr;
	void* ptr = luaL_testudata(ls, n, "MetaWall");
	if (ptr != nullptr)
	{
		wallPtr = *(Wall**)ptr;
	}
	return wallPtr;
}

int WallDestroy(lua_State* ls)
{
	Wall* wall = l_CheckWall(ls, 1);
	delete wall;
	return 0;
}

int WallPrint(lua_State* ls)
{
	Wall* wall = l_CheckWall(ls, 1);
	wall->Print();
	return 0;
}

int WallSetPosition(lua_State* ls)
{
	Wall* wall = l_CheckWall(ls, 1);
	wall->SetPosition(lua_tointeger(ls, 2), lua_tointeger(ls, 3));
	return 0;
}

int WallGetColor(lua_State* ls)
{
	Wall* wall = l_CheckWall(ls, 1);
	lua_pushinteger(ls, wall->GetColor());
	return 1;
}

int WallCheckCollision(lua_State* ls)
{
	Wall* wall = l_CheckWall(ls, 1);
	if (wall->CheckCollision(lua_tointeger(ls, 2), lua_tointeger(ls, 3)))
		lua_pushboolean(ls, true);
	else
		lua_pushboolean(ls, false);
	return 1;
}

void RegisterWall(lua_State* ls)
{
	luaL_newmetatable(ls, "MetaWall");
	luaL_Reg wallRegs[] =
	{
		{"New",				WallCreate},
		{"Print",			WallPrint},
		{"SetPosition",		WallSetPosition},
		{"GetColor",		WallGetColor},
		{"CheckCollision",	WallCheckCollision},
		{"__gc",			WallDestroy},
		{NULL, NULL}
	};
	luaL_setfuncs(ls, wallRegs, 0);
	lua_pushvalue(ls, -1);
	lua_setfield(ls, -1, "__index");
	lua_setglobal(ls, "Wall");
}