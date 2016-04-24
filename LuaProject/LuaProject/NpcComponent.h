#pragma once
#include "Component.h"
#include<string>
class NpcComponent : public Component
{
public:
	NpcComponent() {};

	void setPhrase(const std::string& phrase) {
		this->phrase = phrase;
	}

	std::string getPhrase() const {
		return phrase;
	}
private:
	std::string phrase;
};