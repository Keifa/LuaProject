#include "Player.h"

Player::Player()
{
}

Player::Player(const std::string name)
{
	this->name = name;
	x = 0;
	y = 0;
}

Player::~Player()
{
}

void Player::print()
{
	std::cout	<< "[C++] Player name: " << name
				<< "\tPos: " << x << ", " << y << "\n";
}

void Player::setPosition(int x, int y)
{
	this->x = x;
	this->y = y;
}
