#pragma once
#include <iostream>
#include <string>
#include "lua.hpp"
class Entity
{
private:
	std::string _texture;
	int _x;
	int _y;
public:
	Entity(const std::string texture)
	{
		_texture = texture;
	}

	std::string GetTextureString()
	{
		return this->_texture;
	}
	void SetPosition(int x, int y)
	{
		_x = x;
		_y = y;
		std::cout << "[c++] Set Position\n";
	}
	void SetTextureString(std::string texture)
	{
		this->_texture = texture;
	}

	void Move(int x, int y)
	{
		_x += x;
		_y += y;
	}

	int GetX()
	{
		return _x;
	}

	int GetY()
	{
		return _y;
	}

	bool CollisionCheck(int x, int y)
	{
		bool check = false;
		if (_x == x && _y == y)
			check = true;
		return check;
	}
};

int entity_create(lua_State* ls)
{
	const char* texture = lua_tolstring(ls, 1, nullptr);
	if (texture != nullptr)
	{
		Entity** entity = reinterpret_cast<Entity**>(lua_newuserdata(ls, sizeof(Entity*)));
		*entity = new Entity(texture);

		luaL_getmetatable(ls, "MetaEntity");
		lua_setmetatable(ls, -2);

		std::cout << "[c++] Created Entity\n";
	}
	else {
		//something is fucked up
	}
	return 1;
}

Entity* l_CheckEntity(lua_State* ls, int n)
{
	Entity* EntityPtr = nullptr;

	void* ptr = luaL_testudata(ls, n, "MetaEntity");
	if (ptr != nullptr)
	{
		EntityPtr = *(Entity**)ptr;
	}

	return EntityPtr;
}

int entity_destroy(lua_State* ls)
{
	Entity* entity = l_CheckEntity(ls, 1);
	delete entity;

	std::cout << "[c++] Deleted Entity\n";

	return 0;
}

int entity_getX(lua_State* ls)
{
	Entity* entity = l_CheckEntity(ls, 1);

	lua_pushinteger(ls, entity->GetX());
	return 1;
}

int entity_getY(lua_State* ls)
{
	Entity* entity = l_CheckEntity(ls, 1);

	lua_pushinteger(ls, entity->GetY());
	return 1;
}
int entity_move(lua_State* ls)
{
	Entity* entity = l_CheckEntity(ls, 1);
	entity->Move(lua_tointeger(ls, 2), lua_tointeger(ls, 3));
	return 0;
}

int entity_setPosition(lua_State* ls)
{
	Entity* entity = l_CheckEntity(ls, 1);
	entity->SetPosition(lua_tointeger(ls, 2), lua_tointeger(ls, 3));
	return 0;
}

int entity_collisionCheck(lua_State* ls)
{
	bool check = false;
	Entity* entity = l_CheckEntity(ls, 1);
	if (entity->CollisionCheck(lua_tointeger(ls, 2), lua_tointeger(ls, 3)))
		check = true;
	lua_pushboolean(ls, check);
	return 1;
}
int entity_getTextureString(lua_State* ls)
{
	Entity* entity = l_CheckEntity(ls, 1);

	lua_pushstring(ls, entity->GetTextureString().c_str());
	return 1;
}
int entity_setTextureString(lua_State* ls)
{
	Entity* entity = l_CheckEntity(ls, 1);

	const char* texture = lua_tolstring(ls, 2, nullptr);
	entity->SetTextureString(texture);
	return 1;
}

void RegisterEntity(lua_State* ls)
{
	luaL_newmetatable(ls, "MetaEntity");

	luaL_Reg sEntityRegs[] =
	{
		{ "New",			entity_create },
		{ "SetPosition",	entity_setPosition },
		{ "Move",			entity_move},
		{ "GetX",			entity_getX },
		{ "GetY",			entity_getY },
		{ "CollisionCheck",	entity_collisionCheck },
		{ "GetTexture",		entity_getTextureString},
		{ "SetTexture",		entity_setTextureString },
		{ "__gc",			entity_destroy },
		{ NULL, NULL }
	};

	luaL_setfuncs(ls, sEntityRegs, 0);

	lua_pushvalue(ls, -1);

	lua_setfield(ls, -1, "__index");

	lua_setglobal(ls, "Entity");
}