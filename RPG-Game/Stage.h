#pragma once
#include <SFML/Graphics.hpp>
#include <iostream>
#include <vector>
#include <map>
#include "Tile.h"
#include "FloorTile.h"
#include "WallTile.h"
#include "Player.h"
#define _USE_MATH_DEFINES
#include <math.h>

class Stage : public AbstractEntity
{
private:
	int stage[18][33] = { // tiene uno extra para fixear la caida
		{ 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
		{ 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
		{ 1,1,1,1,1,1,1,1,1,1,1,1,1,1,0,0,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1},
		{ 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
		{ 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
		{ 0,0,0,0,0,0,0,0,0,0,0,1,1,1,1,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
		{ 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
		{ 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
		{ 1,0,0,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,0,0,1,1,1,1,1,1,1,1,1},
		{ 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,2,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
		{ 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,2,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
		{ 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,2,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
		{ 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,2,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
		{ 1,1,1,1,1,1,1,1,1,1,1,0,0,1,1,1,1,1,0,0,1,1,1,1,1,1,1,1,1,1,1,0,0},
		{ 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
		{ 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
		{ 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
		{ 1,1,1,1,0,0,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1},

	};

	std::vector<Tile*> tiles;

public:
	Stage() {
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

	void Draw(sf::RenderWindow& window) {
		for (auto item : tiles) {
			item->Draw(window);
		}
	}

	void Update() {

	}

	const std::vector<Tile*> GetTiles() {
		return tiles;
	}

	void checkCollisions(Player& player) {
		sf::Vector2f currPosPlayer = player.GetPos();

		sf::Vector2f newPos = currPosPlayer;

		for (auto tile : tiles) {
			auto tileBounds = tile->GetGlobalBounds();
			auto tilePos = tile->GetPosition();

			if (tile->Code() == FloorTile::CODE) {

				if (tileBounds.contains(player.GetPosBottomRight()) || tileBounds.contains(player.GetPosBottomLeft())) {
					auto diff = currPosPlayer - tilePos;

					// Calculate the angle in radians
					double angle_radians = std::atan2(-diff.y, diff.x);
					// Convert radians to degrees
					double angle_degrees = angle_radians * (180.0 / M_PI);

					//esta arriba
					//std::cout << angle_degrees << std::endl;
					if (angle_degrees >= 0 && angle_degrees <= 180) {
						newPos = { newPos.x, tilePos.y - (Tile::DEFAULT_SIZE / 2) };
						player.SetPos(newPos);
					}
				}

			}
			else if (tile->Code() == WallTile::CODE) {
				if ((tileBounds.contains(player.GetPosCenterLeft())) || (tileBounds.contains(player.GetPosCenterRight()))) {
					if (tileBounds.contains(player.GetPosCenterRight())) {
						auto diff = player.GetPosCenterRight() - tilePos;

						// Calculate the angle in radians
						double angle_radians = std::atan2(-diff.y, diff.x);
						// Convert radians to degrees
						double angle_degrees = angle_radians * (180.0 / M_PI);

						//esta a la izquierda
						//std::cout << angle_degrees << std::endl;
						if ((angle_degrees >= 90 && angle_degrees <= 180 || angle_degrees <= -90 && angle_degrees >= -180)) {
							//if (tileBounds.intersects(player.GetGlobalBounds())) {
							//if (true && diffPrev.x < 0) {
							newPos = { tilePos.x - (Tile::DEFAULT_SIZE / 2) - (player.GetWidth() / 2), (newPos.y) };
							player.SetPos(newPos);
						}

					}
					else if (tileBounds.contains(player.GetPosCenterLeft())) {
						auto diff = player.GetPosCenterLeft() - tilePos;

						// Calculate the angle in radians
						double angle_radians = std::atan2(-diff.y, diff.x);
						// Convert radians to degrees
						double angle_degrees = angle_radians * (180.0 / M_PI);

						//esta a la derecha
						//std::cout << angle_degrees << std::endl;
						if (angle_degrees >= -90 && angle_degrees <= 90) {
							newPos = { tilePos.x + (Tile::DEFAULT_SIZE / 2) + (player.GetWidth() / 2), (newPos.y) };
							player.SetPos(newPos);
						}

					}
				}
			}

		}
	
	}
};

