#pragma once
#include <SFML/Graphics.hpp>
#include "Character.h"
#include <vector>
#include "Tile.h"
#include "FloorTile.h"
#include "WallTile.h"
#include "HealthBar.h"

class Enemy : public Character
{
private:

	float max_dist = 0;
	float min_dist = 0;

	bool go_forward = true;

	std::vector<Tile*> tiles;

	float vel = 1.0f;

public:
	
	Enemy() : Character("./img/enemy_spritesheet.png", 66, 63, 1000, 10)
	{
	}

	virtual void Update() {
		if (go_forward) {
			bool is_moving = false;
			for (auto tile : tiles) {
				auto tileBounds = tile->GetGlobalBounds();
				if (tile->Code() == FloorTile::CODE) {
					if (tileBounds.contains(sprite.getPosition())) {
						sprite.move({ vel,0 });
						sprite.setScale({ 1,1 });
						is_moving = true;
					}
				}
				else if (tile->Code() == WallTile::CODE) {
					if (tileBounds.contains(GetPosCenterRight())) {
						is_moving = false;
						break;
					}
				}

			}
			if (!is_moving) {
				go_forward = false;
				sprite.move({ -vel,0 });
			}
		}
		else {
			bool is_moving = false;
			for (auto tile : tiles) {
				auto tileBounds = tile->GetGlobalBounds();
				if (tile->Code() == FloorTile::CODE) {
					if (tileBounds.contains(sprite.getPosition())) {
						sprite.move({ -vel,0 });
						sprite.setScale({ -1,1 });
						is_moving = true;
					}
				}
				else if (tile->Code() == WallTile::CODE) {
					if (tileBounds.contains(GetPosCenterLeft())) {
						is_moving = false;
						break;
					}
				}

			}
			if (!is_moving) {
				go_forward = true;
				sprite.move({ vel,0 });
			}
		}

		//healthbar
		healthBar->SetHealth(health);
		healthBar->SetPosition(sprite.getPosition());
		healthBar->Update();
	};

	Enemy* SetPath(float min, float max) {
		max_dist = max; min_dist = min;
		return this;
	}

	Enemy* SetTiles(const std::vector<Tile*>& tiles_) {
		tiles = tiles_;
		return this;
	}

	Enemy* SetVel(float v) {
		vel = std::abs(v);
		return this;
	}

	Enemy* SetForward(bool isForward) {
		go_forward = isForward;
		return this;
	}

};

