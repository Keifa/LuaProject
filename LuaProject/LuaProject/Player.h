#ifndef PLAYER_H
#define PLAYER_H

#include <string>
#include <iostream>
#include "lua.hpp"
#include "SFML\Graphics.hpp"
#include "ConstVars .h"

class Player
{
public:
	Player();
	Player(const std::string name);
	virtual ~Player();

	void print();
	void setPosition(int x, int y);
	void move(int x, int y);
	void draw();

private:
	std::string name;
	int x, y;
	sf::RectangleShape* rect;
};

#endif