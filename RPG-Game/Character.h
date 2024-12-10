#pragma once
#include <SFML/Graphics.hpp>
#include "AbstractEntity.h"
#include "HealthBar.h"
#include <string>

class Character : public AbstractEntity
{
protected:
	std::string spriteFilePath;

	sf::Sprite sprite;
	sf::Texture texture;
	sf::Image image;

	int width;
	int height;

	int health = 1000;
	int damage = 10;

	sf::IntRect pos_idle;

	float hb_width;
	HealthBar* healthBar;

public:

	Character(std::string _spriteFilePath, int _width, int _height, int _health, int _damage, float _hb_width = 0)
		: spriteFilePath(_spriteFilePath), width(_width), height(_height), health(_health), damage(_damage), hb_width(_hb_width)
	{
		if(hb_width == 0) hb_width = width * .7f;
		pos_idle = sf::IntRect(0, 0, width, height);

		image.loadFromFile(spriteFilePath);
		image.createMaskFromColor(sf::Color::Red);


		texture.loadFromImage(image, pos_idle);
		sprite.setTexture(texture);

		sprite.setOrigin(width / 2, height);
		sprite.setScale({ 1,1 });

		healthBar = new HealthBar(0, 0, hb_width, 4, health);
		healthBar->SetOrigin({ hb_width / 2 , 10.f + height });
	}

	virtual void Draw(sf::RenderWindow& window) override {
		healthBar->Draw(window);
		window.draw(sprite);
	};

	virtual ~Character() {
		if (healthBar != nullptr) delete healthBar;
	}

	void takeDamage(int dmg) {
		health -= dmg;
		if (health < 0) health = 0;
	};
	void Kill() {
		health = 0;
	};
	bool isAlive() { return health > 0; };

	int getHealth() const { return health; };
	int getDamage() const { return damage; };

	sf::Vector2f GetPos() {
		return sprite.getPosition();
	};

	sf::Vector2f GetPosCenterRight() {
		auto currPosPlayer = GetPos();
		return { currPosPlayer.x + (width / 2), currPosPlayer.y - (height / 2) };
	};

	sf::Vector2f GetPosCenterLeft() {
		auto currPosPlayer = GetPos();
		return { currPosPlayer.x - (width / 2), currPosPlayer.y - (height / 2) };
	};

	sf::FloatRect GetGlobalBounds() {
		return sprite.getGlobalBounds();
	};

	virtual Character* SetPos(sf::Vector2<float> pos) {
		sprite.setPosition(pos);
		return this;
	};
};