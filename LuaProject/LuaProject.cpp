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

void InitTextures(sf::Texture* player, sf::Texture* box, sf::Texture* ground, sf::Texture* button);
void DrawEntity(Window& w, sf::Shape& shape, sf::Texture& texture, lua_State* L, const std::string& getEntityString);
void InitLua(lua_State* L);

const int TILE_SIZE = 64;
int main()
{
	sf::RectangleShape shape(sf::Vector2f(TILE_SIZE, TILE_SIZE));
	shape.setFillColor(sf::Color::White);
	shape.setOutlineColor(sf::Color::Black);
	shape.setOutlineThickness(2);

	sf::Texture* playerTexture,* boxTexture,* groundTexture,* buttonTexture;
	InitTextures(playerTexture, boxTexture, groundTexture, buttonTexture);

	enum TileTypes
	{
		NORMAL_GROUND,
		WALL,
		NUM_TILE_TYPES
	};

	std::map<int, std::string> keyPressMap =
	{
		{ sf::Keyboard::Up,		"UP" },
		{ sf::Keyboard::Down,	"DOWN" },
		{ sf::Keyboard::Right,	"RIGHT" },
		{ sf::Keyboard::Left,	"LEFT" },
		{ sf::Keyboard::S,		"S" },
		{ sf::Keyboard::R,		"R" },
		{ sf::Keyboard::Num1,	"1" },
		{ sf::Keyboard::Num2,	"2" },
		{ sf::Keyboard::Num3,	"3" },
	};

	lua_State* L = luaL_newstate();
	InitLua(L);

	std::string tempStr = "";
	int size = 0;
	lua_getglobal(L, "mapSize");
	size = lua_tointeger(L, -1);
	int error = 0;
			
	Window w("window.lua");
	int counter = 0;
	while (w.IsOpen())
	{
		lua_getglobal(L, "CheckLevelDone");
		error = lua_pcall(L, 0, 0, 0);
		if (error)
		{
			std::cerr << lua_tostring(L, -1) << std::endl;
			lua_pop(L, 1);
		}
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
					case NORMAL_GROUND:
						shape.setTexture(&groundTexture);
						shape.setFillColor(sf::Color::White);
						break;
					case WALL:
						shape.setTexture(&groundTexture);
						shape.setFillColor(sf::Color::Black);
						break;
					default:
						std::cout << "[C++] ERROR! UNVALID TILE TYPE!\n";
						break;
					}
				}
				w.Draw(shape);
			}
		}

		DrawEntity(w, shape, *buttonTexture, L, "GetButton");
		DrawEntity(w, shape, *boxTexture, L, "GetBox");
		DrawEntity(w, shape, *playerTexture, L, "GetPlayer");
		
		w.Display();
		counter += 1;
	}

	lua_close(L);
	return 0;
}

void DrawEntity(Window& w, sf::Shape& shape, sf::Texture& texture, lua_State* L, const std::string& getEntityString)
{
	lua_getglobal(L, getEntityString.c_str());
	int error = lua_pcall(L, 0, 3, 0);
	if (error)
	{
		std::cerr << lua_tostring(L, -1) << std::endl;
		lua_pop(L, 1);
	}
	else if (!error)
	{
		shape.setPosition(TILE_SIZE * lua_tointeger(L, -2), TILE_SIZE * lua_tointeger(L, -1));
		shape.setTexture(&texture);
		shape.setFillColor(sf::Color::White);
		w.Draw(shape);
		lua_pop(L, 3);
	}
}

void InitTextures(sf::Texture* player, sf::Texture* box, sf::Texture* ground, sf::Texture* button)
{
	if (!player->loadFromFile("Player.png"))
		throw std::runtime_error("Could not load image.png");
	if (!box->loadFromFile("Box.png"))
		throw std::runtime_error("Could not load image.png");
	if (!ground->loadFromFile("ground.png"))
		throw std::runtime_error("Could not load image.png");
	if (!button->loadFromFile("button.png"))
		throw std::runtime_error("Could not load image.png");
}

void InitLua(lua_State* L)
{
	luaL_openlibs(L);

	RegisterEntity(L);
	int error = luaL_loadfile(L, "map.lua") ||
		lua_pcall(L, 0, 0, 0);

	if (error)
	{
		std::cerr << lua_tostring(L, -1) << std::endl;
		lua_pop(L, 1);
	}

	error = luaL_loadfile(L, "handleLevelChange.lua") ||
		lua_pcall(L, 0, 0, 0);
	if (error)
	{
		std::cerr << lua_tostring(L, -1) << std::endl;
		lua_pop(L, 1);
	}
}