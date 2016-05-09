// LuaProject.cpp : Defines the entry point for the console application.
//
#include <iostream>
#include <SFML/Graphics.hpp>
#include "lua.hpp"
#include "Player.h"
#include "helper.h"
#include "Window.h"
#include <string>
#include "Entity.h"

const int TILE_SIZE = 64;
int main()
{
	//sf::RenderWindow window(sf::VideoMode(512, 512), "SFML works!");
	
	sf::RectangleShape shape(sf::Vector2f(TILE_SIZE, TILE_SIZE));
	shape.setFillColor(sf::Color::White);
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
	RegisterEntity(L);
	int error = luaL_loadfile(L, "map.lua") ||
		lua_pcall(L, 0, 0, 0);

	if (error)
	{
		std::cerr << lua_tostring(L, -1) << std::endl;
		lua_pop(L, 1);
	}
	


	std::string tempStr = "";
	int size = 0;
	lua_getglobal(L, "mapSize");
	size = lua_tointeger(L, -1);
			
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
				lua_pushinteger(L, event.mouseButton.x / TILE_SIZE + 1);
				lua_pushinteger(L, event.mouseButton.y / TILE_SIZE + 1);
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
		sf::Texture texture;
		if (!texture.loadFromFile("ground.png"))
		{
			throw std::runtime_error("Could not load image.png");
		}

		shape.setTexture(&texture);
		for (int y = 0; y < size; y++)
		{
			for (int x = 0; x < size; x++)
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
						shape.setFillColor(sf::Color::White);
						break;
					case 1:
						shape.setFillColor(sf::Color::Black);
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
		if (!texture.loadFromFile("Player.png"))
		{
			throw std::runtime_error("Could not load image.png");
		}

		shape.setTexture(&texture);

		lua_getglobal(L, "GetPlayerX");
		error = lua_pcall(L, 0, 1, 0);
		if (error)
		{
			std::cerr << lua_tostring(L, -1) << std::endl;
			lua_pop(L, 1);
		}
		int x = lua_tonumber(L, -1);
		lua_pop(L, 1);

		lua_getglobal(L, "GetPlayerY");
		error = lua_pcall(L, 0, 1, 0);
		if (error)
		{
			std::cerr << lua_tostring(L, -1) << std::endl;
			lua_pop(L, 1);
		}
		int y = lua_tonumber(L, -1);
		lua_pop(L, 1);

		shape.setPosition(TILE_SIZE * (x - 1), TILE_SIZE * (y - 1));

		w.Draw(shape);
		w.Display();

		counter += 1;
	}

	lua_close(L);
	return 0;
}