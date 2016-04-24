#pragma once
#include "Component.h"
#include <string>
class GraphicsComponent : public Component
{
public:
	GraphicsComponent() {};

	void setFilename(const std::string& filename) {
		this->filename = filename;
	}
	std::string getFilename() const {
		return filename;
	}

private:
	std::string filename;
};