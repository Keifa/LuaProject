#ifndef GAME_H
#define GAME_H

#include "include\SFML\Graphics.hpp"
#include "ConstVars .h"
#include "LuaHandler.h"

class Game
{
public:
	Game();
	virtual ~Game();
	void start();

private:
	//sf::RenderWindow* window;
	sf::Event* events;
	LuaHandler lh;
	sf::RectangleShape* test;
	bool check;

	void handleEvents();
	void handleKeyboardEvents();
};

#endif