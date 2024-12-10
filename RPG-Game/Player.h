#pragma once
#include <SFML/Graphics.hpp>
#include <cmath>
#include <vector>
#include "Character.h"
#include "Tile.h"
#include "FloorTile.h"

class Player : public Character
{
private:
	sf::Sprite sword_sprite;
	sf::Texture sword_texture;

	sf::IntRect pos_walk;
	sf::IntRect pos_attk;

	float vel = 0.f;
	float acc_default = 100.f;
	float deacc_default = 200.f;
	float max_vel = 150.f;

	float vel_up = 0.f;
	float max_vel_up = -700.f;

	bool to_right = true;
	bool is_attacking = false;
	bool is_falling = true;

	int tick_changepos = 0;

	int health = 3000;
	int damage = 20;

public:

	Player() : Character("./img/siegward_spritesheet.png", 77, 63, 3000, 20, 80){
		pos_walk = sf::IntRect(((width) * 1), 0, width, height);
		pos_attk = sf::IntRect(((width) * 2), 0, width, height);

		sf::Image swordImage;
		swordImage.loadFromFile("./img/sword.png");
		swordImage.createMaskFromColor(sf::Color::Red);

		sword_texture.loadFromImage(swordImage);
		sword_sprite.setTexture(sword_texture);

		sword_sprite.setOrigin(-(width / 2.5f), (sword_sprite.getLocalBounds().height / 2));
	};

	void Draw(sf::RenderWindow& window) {
		Character::Draw(window);
		window.draw(sword_sprite);
	};

	void Update();
	void ApplyGravity();

	bool isLookingToRight() {
		return to_right;
	};
	bool isLookingToLeft() {
		return !to_right;
	};

	void SetIsFalling(bool isfalling) { is_falling = isfalling; };

	void SetVel(float vel) {
		this->vel = vel;
	};

	bool isFalling() {
		return is_falling;
	}

	sf::Vector2f GetPosCenterRight() {
		auto currPosPlayer = GetPos();
		return { currPosPlayer.x + (GetWidth() / 2), currPosPlayer.y - (GetHeight() / 2) };
	};

	sf::Vector2f GetPosCenterLeft() {
		auto currPosPlayer = GetPos();
		return { currPosPlayer.x - (GetWidth() / 2), currPosPlayer.y - (GetHeight() / 2) };
	};

	void checkFallingState(std::vector<Tile*> tiles) {
		bool player_is_falling = true;
		for (auto tile : tiles) {
			if (tile->Code() == FloorTile::CODE) {
				auto tileBounds = tile->GetGlobalBounds();
				if (tileBounds.contains(GetPosBottomLeft()) || tileBounds.contains(GetPosBottomRight())) {
					player_is_falling = false;
					break;
				}
			}
		}
		is_falling = player_is_falling;
	}

	bool isHittedBySword(sf::FloatRect bounds) {
		return sword_sprite.getGlobalBounds().intersects(bounds);
	}
};

