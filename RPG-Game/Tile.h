#pragma once
#include <SFML/Graphics.hpp>
#include "AbstractEntity.h"

class Tile : public AbstractEntity
{
public:	
	Tile(float posx = 0, float posy = 0) : shape(sf::Vector2f(DEFAULT_SIZE, DEFAULT_SIZE))
	{
		int originX, originY;
		originX = originY = (DEFAULT_SIZE / 2);
		shape.setOrigin(originX, originY);
		shape.setPosition({ posx + originX, posy + originY });
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

	virtual int Code() = 0;

	static const int DEFAULT_SIZE = 40;

protected:
	sf::RectangleShape shape;

};

