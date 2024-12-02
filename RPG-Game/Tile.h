#pragma once
#include <SFML/Graphics.hpp>
#include "AbstractEntity.h"

class Tile : public AbstractEntity
{
public:	
	Tile(float posx = 0, float posy = 0) : shape(sf::Vector2f(DEFAULT_SIZE, DEFAULT_SIZE))
	{
		shape.setFillColor(sf::Color::Green);
		shape.setPosition({ posx, posy });
	}

	void Draw(sf::RenderWindow& window) {
		window.draw(shape);
	}

	void Update() {
	}

	sf::FloatRect GetGlobalBounds() {
		return shape.getGlobalBounds();
	}

	sf::Vector2f GetPosition() const {
		return shape.getPosition();
	}



	static const int DEFAULT_SIZE = 40;

private:
	sf::RectangleShape shape;
};

