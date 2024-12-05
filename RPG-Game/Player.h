#pragma once
#include <SFML/Graphics.hpp>
#include <cmath>
#include <vector>
#include "AbstractEntity.h"
#include "Tile.h"
#include "FloorTile.h"

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
	float max_vel = 150.f;

	float vel_up = 0.f;
	float max_vel_up = -600.f;

	bool to_right = true;
	bool is_attacking = false;
	bool is_falling = true;

	int tick_changepos = 0;
	void SetVel(float vel);

	sf::Vector2f prevPos;

	int health = 3000;
	int damage = 20;

public:
	int GetDrawOrder() override { return 2; };

	Player();
	void Draw(sf::RenderWindow& window);
	void Update();
	void SetPos(sf::Vector2<float> pos);
	void ApplyGravity();
	bool isLookingToRight();
	bool isLookingToLeft();

	int GetWidth() const { return width; }
	int GetHeight() const { return height; }

	void SetIsFalling(bool isfalling) { is_falling = isfalling; };

	sf::Vector2f GetPos();
	sf::Vector2f GetPrevPos() const { return prevPos; }

	sf::FloatRect GetGlobalBounds() {
		return sprite.getGlobalBounds();
	}

	int getHealth() const { return health; };
	int getDamage() const { return damage; };
	void takeDamage(int dmg) { 
		health -= dmg;
		if (health < 0) health = 0;
	};
	void Kill() {
		health = 0;
	};

	bool isAlive() { return health > 0; };

	bool isFalling() {
		return is_falling;
	}

	sf::Vector2f GetPosBottomRight(){ 
		auto currPosPlayer = GetPos();
		return { currPosPlayer.x + (GetWidth() / 4), currPosPlayer.y };
	};

	sf::Vector2f GetPosBottomLeft() {
		auto currPosPlayer = GetPos();
		return { currPosPlayer.x - (GetWidth() / 4), currPosPlayer.y };
	};

	sf::Vector2f GetPosCenter() {
		auto currPosPlayer = GetPos();
		return { currPosPlayer.x, currPosPlayer.y - (GetHeight() / 2) };
	};

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

