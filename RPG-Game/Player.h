#pragma once
#include <SFML/Graphics.hpp>
#include <cmath>
#include <vector>
#include "AbstractEntity.h"

class Player : public AbstractEntity
{
private:
	sf::Sprite sprite;
	sf::Sprite sword_sprite;
	sf::Texture texture;
	sf::Texture sword_texture;
	sf::Image image;

	sf::IntRect pos_current;
	sf::IntRect pos_idle;
	sf::IntRect pos_walk;
	sf::IntRect pos_attk;
	//sf::IntRect pos_walk_l;

	int width = 77;
	int height = 63;

	float vel = 0.f;
	float acc_default = 100.f;
	float deacc_default = 200.f;
	float max_vel = 300.f;

	bool to_right = true;
	bool is_attacking = false;

	int tick_changepos = 0;
	void SetVel(float vel);

public:
	Player();
	void Draw(sf::RenderWindow& window);
	void Update();
	void SetPos(sf::Vector2<float> pos);
	bool isLookingToRight();
	bool isLookingToLeft();

	int GetWidth() const { return width; }
	int GetHeight() const { return height; }

	sf::Vector2f GetPos();

	sf::FloatRect GetGlobalBounds() {
		return sprite.getGlobalBounds();
	}
};

