// LuaProject.cpp : Defines the entry point for the console application.
//
#include <iostream>
#include <SFML/Graphics.hpp>
#include "lua.hpp"
#include "Player.h"
#include "helper.h"
#include "Window.h"
#include <string>

const int TILE_SIZE = 64;
int main()
{
	//sf::RenderWindow window(sf::VideoMode(512, 512), "SFML works!");
	
	sf::RectangleShape shape(sf::Vector2f(TILE_SIZE, TILE_SIZE));
	shape.setFillColor(sf::Color::Green);
	shape.setOutlineColor(sf::Color::Black);
	shape.setOutlineThickness(2);
	
	std::map<int, std::string> keyPressMap =
	{
		{ sf::Keyboard::Up,		"UP" },
		{ sf::Keyboard::Down,	"DOWN" },
		{ sf::Keyboard::Right,	"RIGHT" },
		{ sf::Keyboard::Left,	"LEFT" },
		{ sf::Keyboard::S,		"S" },
		{ sf::Keyboard::R,		"R" }
	};

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

	std::string tempStr = "";
			
	Window w("window.lua");
	int counter = 0;
	while (w.IsOpen())
	{
		sf::Event event;
		while (w.PollEvent(event))
		{
			switch (event.type)
			{
			case sf::Event::Closed:
				w.Close();
				break;
			case sf::Event::KeyPressed:
				tempStr = keyPressMap[event.key.code];
				if (tempStr != "")
				{
					lua_getglobal(L, "HandleKeyPress");
					lua_pushstring(L, tempStr.c_str());
					error = lua_pcall(L, 1, 0, 0);
					if (error)
					{
						std::cerr << lua_tostring(L, -1) << std::endl;
						lua_pop(L, 1);
					}
				}
				else
					std::cout << "Invalid input\n";
				break;
			case sf::Event::MouseButtonPressed:
				lua_getglobal(L, "Clicked");
				lua_pushinteger(L, event.mouseButton.x / 64 + 1);
				lua_pushinteger(L, event.mouseButton.y / 64 + 1);
				error = lua_pcall(L, 2, 0, 0);
				if (error)
				{
					std::cerr << lua_tostring(L, -1) << std::endl;
					lua_pop(L, 1);
				}
				break;
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