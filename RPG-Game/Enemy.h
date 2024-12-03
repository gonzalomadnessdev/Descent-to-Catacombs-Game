#pragma once
#include <SFML/Graphics.hpp>
#include "AbstractEntity.h"
#include "Stage.h"
#include "Tile.h"
#include <vector>

class Enemy : public AbstractEntity
{
private:
	sf::Sprite sprite;
	sf::Texture texture;
	sf::Image image;

	int width = 66;
	int height = 63;

	sf::IntRect pos_idle;

	float max_dist = 0;
	float min_dist = 0;

	bool go_forward = true;

	std::vector<Tile*> tiles;

	float vel = 1.0f;

public:
	
	Enemy()
	{
		pos_idle = sf::IntRect(0, 0, width, height);

		image.loadFromFile("./img/enemy_spritesheet.png");
		image.createMaskFromColor(sf::Color::Red);


		texture.loadFromImage(image, pos_idle);
		sprite.setTexture(texture);

		sprite.setOrigin(width / 2, height);
		sprite.setScale({ 1,1 });

	}

	//sf::Vector2f GetPos() {
	//	return sprite.getPosition();
	//}

	sf::Vector2f GetPosCenterRight() {
		auto currPosPlayer = GetPos();
		return { currPosPlayer.x + (width / 2), currPosPlayer.y - (height / 2) };
	};

	sf::Vector2f GetPosCenterLeft() {
		auto currPosPlayer = GetPos();
		return { currPosPlayer.x - (width / 2), currPosPlayer.y - (height / 2) };
	};

	virtual void Draw(sf::RenderWindow& window) {
		window.draw(sprite);
	};

	virtual void Update() {

		//auto pos = GetPos();
		//if (go_forward) {
		//	if (pos.x == max_dist) {
		//		go_forward = false;
		//		return;
		//	}
		//	sprite.move({ 1,0 });
		//	sprite.setScale({ 1,1 });
		//}
		//else {
		//	if (pos.x == min_dist) {
		//		go_forward = true;
		//		return;
		//	}
		//	sprite.move({ -1,0 });
		//	sprite.setScale({ -1,1 });
		//}

		//pos.y += 20;
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
	};

	Enemy* SetPos(sf::Vector2<float> pos) {
		sprite.setPosition(pos);
		return this;
	}

	sf::Vector2f GetPos() {
		return sprite.getPosition();
	}

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

