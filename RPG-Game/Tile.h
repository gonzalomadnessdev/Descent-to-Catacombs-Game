#pragma once
#include <SFML/Graphics.hpp>
#include "AbstractEntity.h"

class Tile : public AbstractEntity
{
public:	
	Tile(float posx = 0, float posy = 0) 
	{

		image.loadFromFile("./img/block.png");
		image.createMaskFromColor(sf::Color::Red);
		texture.loadFromImage(image);
		sprite.setTexture(texture);

		int originX, originY;
		originX = originY = (DEFAULT_SIZE / 2);
		sprite.setOrigin(originX, originY);
		sprite.setPosition({ posx + originX, posy + originY });
	}

	void Draw(sf::RenderWindow& window) {
		window.draw(sprite);
	}

	void Update() {
	}

	sf::FloatRect GetGlobalBounds() {
		return sprite.getGlobalBounds();
	}

	sf::Vector2f GetPosition() const {
		return sprite.getPosition();
	}

	virtual int Code() = 0;

	static const int DEFAULT_SIZE = 40;

protected:
	//sf::RectangleShape shape;

	sf::Texture texture;
	sf::Image image;
	sf::Sprite sprite;

};

