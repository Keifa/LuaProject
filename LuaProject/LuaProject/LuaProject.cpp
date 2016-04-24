// LuaProject.cpp : Defines the entry point for the console application.
//
#include <iostream>
#include <SFML/Graphics.hpp>
#include "lua.hpp"
#include "Player.h"

int main()
{
	sf::RenderWindow window(sf::VideoMode(720, 480), "SFML works!");

	sf::CircleShape shape(100.f);
	shape.setFillColor(sf::Color::Green);

	lua_State* L = luaL_newstate();

	if (L)
	{
		luaL_openlibs(L);

		RegisterPlayer(L);

		int error = luaL_loadfile(L, "script.lua") ||
			lua_pcall(L, 0, 0, 0);

		if (error)
		{
			std::cerr << lua_tostring(L, -1) << std::endl;
			lua_pop(L, 1);
		}
	}
	int x, y;
	lua_getglobal(L, "getX");
	int error = lua_pcall(L, 0, 1, 0);
	if (!error)
	{
		x = lua_tonumber(L, -1);
		lua_pop(L, 1);
	}
	lua_getglobal(L, "getY");
	error = lua_pcall(L, 0, 1, 0);
	if (!error)
	{
		y = lua_tonumber(L, -1);
		lua_pop(L, 1);
	}
	shape.setPosition(x, y);

	int counter = 0;
	while (window.isOpen())
	{
		sf::Event event;
		while (window.pollEvent(event))
		{
			if (event.type == sf::Event::Closed)
				window.close();
		}
		/*lua_getglobal(L, "update");
		lua_pushinteger(L, counter);
		error = lua_pcall(L, 1, 0, 0);
		if (error)
		{
			std::cerr << lua_tostring(L, -1) << std::endl;
			lua_pop(L, 1);
		}*/
		if (counter % 10 == 1)
		{
			if (sf::Keyboard::isKeyPressed(sf::Keyboard::W))
			{
				lua_getglobal(L, "movePlayerY");
				lua_pushinteger(L, -1);
				error = lua_pcall(L, 1, 0, 0);
				if (error)
				{
					std::cerr << lua_tostring(L, -1) << std::endl;
					lua_pop(L, 1);
				}
			}
			if (sf::Keyboard::isKeyPressed(sf::Keyboard::S))
			{
				lua_getglobal(L, "movePlayerY");
				lua_pushinteger(L, 1);
				error = lua_pcall(L, 1, 0, 0);
				if (error)
				{
					std::cerr << lua_tostring(L, -1) << std::endl;
					lua_pop(L, 1);
				}
			}
			if (sf::Keyboard::isKeyPressed(sf::Keyboard::D))
			{
				lua_getglobal(L, "movePlayerX");
				lua_pushinteger(L, 1);
				error = lua_pcall(L, 1, 0, 0);
				if (error)
				{
					std::cerr << lua_tostring(L, -1) << std::endl;
					lua_pop(L, 1);
				}
			}
			if (sf::Keyboard::isKeyPressed(sf::Keyboard::A))
			{
				lua_getglobal(L, "movePlayerX");
				lua_pushinteger(L, -1);
				error = lua_pcall(L, 1, 0, 0);
				if (error)
				{
					std::cerr << lua_tostring(L, -1) << std::endl;
					lua_pop(L, 1);
				}
			}
		}
		lua_getglobal(L, "getX");
		int error = lua_pcall(L, 0, 1, 0);
		if (!error)
		{
			x = lua_tonumber(L, -1);
			lua_pop(L, 1);
		}
		//std::cout << x << "X\n";
		lua_getglobal(L, "getY");
		error = lua_pcall(L, 0, 1, 0);
		if (!error)
		{
			y = lua_tonumber(L, -1);
			lua_pop(L, 1);
		}

		//std::cout << y << "Y\n";
		shape.setPosition(x, y);

		window.clear();
		window.draw(shape);
		window.display();

		counter += 1;
	}
	lua_close(L);
	return 0;
}

