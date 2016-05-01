// LuaProject.cpp : Defines the entry point for the console application.
//
#include <iostream>
#include <SFML/Graphics.hpp>
#include "lua.hpp"
#include "Player.h"
#include "helper.h"
#include "Window.h"

const int TILE_SIZE = 64;
int main()
{
	//sf::RenderWindow window(sf::VideoMode(512, 512), "SFML works!");

	sf::RectangleShape shape(sf::Vector2f(TILE_SIZE, TILE_SIZE));
	shape.setFillColor(sf::Color::Green);
	shape.setOutlineColor(sf::Color::Black);
	shape.setOutlineThickness(2);

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

	int error = luaL_loadfile(L, "map.lua") ||
		lua_pcall(L, 0, 0, 0);

	if (error)
	{
		std::cerr << lua_tostring(L, -1) << std::endl;
		lua_pop(L, 1);
	}

	Window w("window.lua");
	int counter = 0;
	while (w.IsOpen())
	{
		sf::Event event;
		while (w.PollEvent(event))
		{
			if (event.type == sf::Event::Closed)
				w.Close();
			if (sf::Keyboard::isKeyPressed(sf::Keyboard::Escape))
			{
				w.Close();
			}
			if (sf::Keyboard::isKeyPressed(sf::Keyboard::R))
			{
				lua_getglobal(L, "Load");
				error = lua_pcall(L, 0, 0, 0);
				if (error)
				{
					std::cerr << lua_tostring(L, -1) << std::endl;
					lua_pop(L, 1);
				}
			}
			if (sf::Keyboard::isKeyPressed(sf::Keyboard::T))
			{
				lua_getglobal(L, "Save");
				error = lua_pcall(L, 0, 0, 0);
				if (error)
				{
					std::cerr << lua_tostring(L, -1) << std::endl;
					lua_pop(L, 1);
				}
			}
			if (event.type == sf::Event::MouseButtonPressed)
			{
				lua_getglobal(L, "Clicked");
				lua_pushinteger(L, event.mouseButton.x/64 + 1);
				lua_pushinteger(L, event.mouseButton.y/64 + 1);
				error = lua_pcall(L, 2, 0, 0);
				if (error)
				{
					std::cerr << lua_tostring(L, -1) << std::endl;
					lua_pop(L, 1);
				}
			}
		}
		//window.clear();
		//window.draw(shape);

		for (int y = 0; y < 8; y++)
		{
			for (int x = 0; x < 8; x++)
			{
				shape.setPosition(TILE_SIZE * x, TILE_SIZE * y);

				lua_getglobal(L, "GetTile");
				lua_pushinteger(L, x+1);
				lua_pushinteger(L, y+1);

				error = lua_pcall(L, 2, 1, 0);
				if (error)
				{
					std::cerr << lua_tostring(L, -1) << std::endl;
					lua_pop(L, 1);
				}
				else if (!error)
				{
					int x = lua_tonumber(L, -1);
					lua_pop(L, 1);

					switch (x)
					{
					case 0:
						shape.setFillColor(sf::Color::Blue);
						break;
					case 1:
						shape.setFillColor(sf::Color::Green);
						break;
					case 2:
						shape.setFillColor(sf::Color::Red);
						break;
					case 3:
						shape.setFillColor(sf::Color::Yellow);
						break;
					default:
						break;
					}
				}
				w.Draw(shape);
			}
		}

		w.Display();

		counter += 1;
	}

	lua_close(L);
	return 0;
}

