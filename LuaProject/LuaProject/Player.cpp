#include "Player.h"

Player::Player()
{
	this->name = "";
	x = 0;
	y = 0;
	rect = new sf::RectangleShape(sf::Vector2f(100, 100));
	rect->setFillColor(sf::Color(0, 255, 0));
	rect->setPosition(sf::Vector2f(x, y));
}

Player::Player(const std::string name)
{
	this->name = name;
	x = 0;
	y = 0;
	rect = new sf::RectangleShape(sf::Vector2f(100, 100));
	rect->setFillColor(sf::Color(0, 255, 0));
	rect->setPosition(sf::Vector2f(x, y));
}

Player::~Player()
{
	delete rect;
}

void Player::print()
{
	std::cout	<< "[C++] Player name: " << name
				<< " Pos: " << x << ", " << y << "\n";
}

void Player::setPosition(int x, int y)
{
	this->x = x;
	this->y = y;
	rect->setPosition(sf::Vector2f(x, y));
}

void Player::move(int x, int y)
{
	this->x += x;
	this->y += y;
	rect->setPosition(sf::Vector2f(this->x, this->y));
}

void Player::draw()
{
	//window::renderWindow->draw(*rect);
}
