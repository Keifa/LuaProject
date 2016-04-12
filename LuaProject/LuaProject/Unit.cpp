#include "Unit.h"

Unit::Unit(const std::string name)
{
	this->name = name;
	this->x = 0.0;
	this->y = 0.0;
}

//Unit::Unit(const std::string name, float x, float y)
//{
//	this->name = name;
//	this->x = x;
//	this->y = y;
//}

void Unit::print()
{
	std::cout	<< "[C++][Unit]\tUnit name: " << this->name
				<< "\n\t\tPos: " << this->x << ", " << this->y << "\n";
}

void Unit::move(float x, float y)
{
	this->x += x;
	this->y += y;
}

void Unit::setPosition(float x, float y)
{
	this->x = x;
	this->y = y;
}
