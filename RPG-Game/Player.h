#pragma once
#include <SFML/Graphics.hpp>

class Player
{
private:
	sf::Sprite sprite;
	sf::Texture texture;

public:
	Player()
	{

	}

	void Draw(sf::RenderWindow& window) {
		window.draw(sprite);
	}

	void Update() {
		sf::Vector2 movement(0.f, 0.f);
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left)) {
			movement.x -= 1.f;
		}
		else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left)) {
			movement.x += 1.f;
		}

		sprite.move(movement);
	}
};

