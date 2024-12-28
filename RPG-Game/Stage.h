#pragma once
#include <SFML/Graphics.hpp>
#include <iostream>
#include <vector>
#include <map>
#include "Tile.h"
#include <SFML/Graphics.hpp>
#include "FloorTile.h"
#include "WallTile.h"
#include "Player.h"
#define _USE_MATH_DEFINES
#include <math.h>
#include "Enemy.h"
#include "FinalBoss.h"
#include <fstream>

struct EnemyRecord {
	int code;
	float x;
	float y;
	float vel;
	bool forward;
	float healthF;
	float damageF;
};

class Stage : public AbstractEntity
{
private:
	int x_max = 33;
	int y_max = 18;
	int stage_num = 1;

	int stage[18][33];

	std::vector<Tile*> tiles;
	sf::Sprite bgSprite;
	sf::Texture bgtexture;

	void LoadStage() {
		std::ifstream file(("data/stages/" + std::to_string(stage_num) + ".stg"), std::ios::binary);
		file.read(reinterpret_cast<char*>(&stage), sizeof(stage));
	}

public:
	Stage(int num) : stage_num(num) {	
		if (num > 0) {
			LoadStage();

			for (size_t i = 0; i < 18; i++)
			{
				for (size_t j = 0; j < 33; j++)
				{
					if (stage[i][j] != 0) {
						//tiles.push_back(Tile((j * Tile::DEFAULT_SIZE), i * Tile::DEFAULT_SIZE));
						if (stage[i][j] == 1) {
							tiles.push_back(new FloorTile((j * Tile::DEFAULT_SIZE), i * Tile::DEFAULT_SIZE));
						}
						else if (stage[i][j] == 2) {
							tiles.push_back(new WallTile((j * Tile::DEFAULT_SIZE), i * Tile::DEFAULT_SIZE));
						}
					}
				}
			}

			std::sort(tiles.begin(), tiles.end(), [](Tile* t1, Tile* t2) { return t1->Code() < t2->Code(); });
		}


		//background
		if (!bgtexture.loadFromFile("./img/background.png"))
		{
			exit(1);
		}

		bgSprite.setTexture(bgtexture);
	}

	void LoadEnemies(std::vector<Character*>& enemies, Player* player) {

		std::ifstream file(("data/enemies/stage_" + std::to_string(stage_num) + ".nmy"), std::ios::binary);

		EnemyRecord er;
		while (file.read(reinterpret_cast<char*>(&er), sizeof(er))) {
			Character* e;
			switch (er.code)
			{
			case FinalBoss::CHARACTER_CODE:
				e = (new FinalBoss(player));
				break;
			case Enemy::CHARACTER_CODE:
			default:
				e = (new Enemy())->SetTiles(tiles)->SetVel(er.vel)->SetForward(er.forward)->SetDamageFactor(er.damageF)->SetHealthFactor(er.healthF);
				break;
			}
			
			e->SetPos({ (float)er.x,(float)er.y });
			enemies.push_back(e);
		}
	}

	void Draw(sf::RenderWindow& window) {
		window.draw(bgSprite);
		for (auto item : tiles) {
			item->Draw(window);
		}
	}

	void Update() {

	}

	const std::vector<Tile*> GetTiles() {
		return tiles;
	}

	void checkCollisions(Player* player) {
		sf::Vector2f currPosPlayer = player->GetPos();

		sf::Vector2f newPos = currPosPlayer;

		for (auto tile : tiles) {
			auto tileBounds = tile->GetGlobalBounds();
			auto tilePos = tile->GetPosition();

			if (tile->Code() == FloorTile::CODE) {

				if (tileBounds.contains(player->GetPosBottomRight()) || tileBounds.contains(player->GetPosBottomLeft())) {
					auto diff = currPosPlayer - tilePos;

					double angle_radians = std::atan2(-diff.y, diff.x);
					double angle_degrees = angle_radians * (180.0 / M_PI);

					//esta arriba
					if (angle_degrees >= 0 && angle_degrees <= 180) {
						newPos = { newPos.x, tilePos.y - (Tile::DEFAULT_SIZE / 2) };
						player->SetPos(newPos);
					}
				}

			}
			else if (tile->Code() == WallTile::CODE) {
				if ((tileBounds.contains(player->GetPosCenterLeft())) || (tileBounds.contains(player->GetPosCenterRight()))) {
					if (tileBounds.contains(player->GetPosCenterRight())) {
						auto diff = player->GetPosCenterRight() - tilePos;

						double angle_radians = std::atan2(-diff.y, diff.x);
						double angle_degrees = angle_radians * (180.0 / M_PI);

						//esta a la izquierda
						if ((angle_degrees >= 90 && angle_degrees <= 180 || angle_degrees <= -90 && angle_degrees >= -180)) {
							newPos = { tilePos.x - (Tile::DEFAULT_SIZE / 2) - (player->GetWidth() / 2), (newPos.y) };
							player->SetPos(newPos);
						}

					}
					else if (tileBounds.contains(player->GetPosCenterLeft())) {
						auto diff = player->GetPosCenterLeft() - tilePos;

						// Calculate the angle in radians
						double angle_radians = std::atan2(-diff.y, diff.x);
						// Convert radians to degrees
						double angle_degrees = angle_radians * (180.0 / M_PI);

						//esta a la derecha
						if (angle_degrees >= -90 && angle_degrees <= 90) {
							newPos = { tilePos.x + (Tile::DEFAULT_SIZE / 2) + (player->GetWidth() / 2), (newPos.y) };
							player->SetPos(newPos);
						}

					}
				}
			}

		}
	
	}
};

