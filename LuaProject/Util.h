#pragma once
#include <map>

std::map<int, std::string> keyPressMap =
{
	{ sf::Keyboard::Up,		"UP" },
	{ sf::Keyboard::Down,	"DOWN" },
	{ sf::Keyboard::Right,	"RIGHT" },
	{ sf::Keyboard::Left,	"LEFT" },
	{ sf::Keyboard::S,		"S" },
	{ sf::Keyboard::R,		"R" }
};

enum Colors
{
	WHITE,
	BLACK,
	GREEN,
	RED,
	YELLOW,
	BLUE
};