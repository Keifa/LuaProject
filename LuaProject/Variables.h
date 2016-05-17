#pragma once

#include <string>
#include <iostream>
#include <SFML/Graphics.hpp>
#include "lua.hpp"
#include "Window.h"
#include "Entity.h"

const int TILE_SIZE = 64;

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

std::map<int, std::string> editorKeyPressMap =
{
	{ sf::Keyboard::S,		"S" },
	{ sf::Keyboard::R,		"R" },
	{ sf::Keyboard::Num1,	"1" },
	{ sf::Keyboard::Num2,	"2" },
	{ sf::Keyboard::Num3,	"3" },
};

enum EntityTypes
{
	PLAYER,
	BOX,
	GOAL,
	GROUNDTILE,

	NUM_ENTITY_TYPES
};

struct EnityStruct
{
	EntityTypes type;
	std::string textureFileName;
	sf::Texture* texture;
	std::string luaGetFunction;
};