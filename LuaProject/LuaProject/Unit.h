#ifndef UNIT_H
#define UNIT_H
#include <string>
#include <iostream>

class Unit
{
private:
	std::string name;
	float x, y;
public:
	Unit(const std::string name);
	//Unit(const std::string name, float x, float y);

	void print();
	void move(float x, float y);
	void setPosition(float x, float y);
};

#endif