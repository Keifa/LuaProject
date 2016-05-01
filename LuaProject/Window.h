#pragma once
#include "lua.hpp"
#include <string>
#include <iostream>
#include <SFML/Graphics.hpp>
class Window
{
public:
	Window(std::string path)
	{
		L = luaL_newstate();
		int error = luaL_loadfile(L, path.c_str()) ||
			lua_pcall(L, 0, 0, 0);

		if (error)
		{
			std::cerr << lua_tostring(L, -1) << std::endl;
			lua_pop(L, 1);
		}

		lua_getglobal(L, "windowTitle");
		lua_getglobal(L, "windowWidth");
		lua_getglobal(L, "windowHeight");

		windowTitle = std::string(lua_tostring(L, -3));
		windowWidth = lua_tointeger(L, -2);
		windowHeight = lua_tointeger(L, -1);

		lua_close(L);

		window.create(sf::VideoMode(windowWidth, windowHeight), windowTitle.c_str());
	}
	~Window(){}
	void Draw(sf::Drawable& drawable)
	{
		window.draw(drawable);
	}
	void Display()
	{
		window.display();
	}
	void Clear()
	{
		window.clear();
	}
	bool IsOpen()
	{
		return window.isOpen();
	}
	bool PollEvent(sf::Event& event)
	{
		return window.pollEvent(event);
	}
	void Close()
	{
		window.close();
	}
private:
	std::string windowTitle;
	int windowWidth, windowHeight;
	int sizeX, sizeY;

	sf::RenderWindow window;
	lua_State* L;
};