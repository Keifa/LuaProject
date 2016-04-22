#ifndef PLAYER_H
#define PLAYER_H

#include <string>
#include <iostream>
#include "lua.hpp"

class Player
{
public:
	Player();
	Player(const std::string name);
	virtual ~Player();

	void print();
	void setPosition(int x, int y);

private:
	std::string name;
	int x, y;
};

#endif