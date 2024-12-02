#pragma once
#include <SFML/Graphics.hpp>

class AbstractEntity
{
private:
public:
	virtual void Draw(sf::RenderWindow& window) = 0;
	virtual void Update() = 0;
	virtual ~AbstractEntity() {

	}
};

