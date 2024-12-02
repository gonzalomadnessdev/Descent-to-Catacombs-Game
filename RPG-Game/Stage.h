#pragma once
#include <SFML/Graphics.hpp>
#include <iostream>
#include <vector>
#include <map>
#include "Tile.h"
#include "Player.h"

class Stage : public AbstractEntity
{
private:
	int stage[18][33] = { // tiene uno extra para fixear la caida
		{ 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
		{ 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
		{ 1,1,1,1,1,1,1,1,1,1,1,1,1,1,0,0,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1},
		{ 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
		{ 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
		{ 0,0,0,0,0,0,0,0,0,0,0,0,0,1,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
		{ 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
		{ 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
		{ 1,0,0,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,0,0,1,1,1,1,1,1,1,1,1},
		{ 0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
		{ 0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
		{ 0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
		{ 0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
		{ 1,1,1,1,1,1,1,1,1,1,1,0,0,1,1,1,1,1,0,0,1,1,1,1,1,1,1,1,1,1,1,0,0},
		{ 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
		{ 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
		{ 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
		{ 1,1,1,1,0,0,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1},

	};

	std::map<std::pair<int, int>,Tile> tiles;

public:
	Stage() {
		for (size_t i = 0; i < 18; i++)
		{
			for (size_t j = 0; j < 33; j++)
			{
				if (stage[i][j] != 0) {
					//tiles.push_back(Tile((j * Tile::DEFAULT_SIZE), i * Tile::DEFAULT_SIZE));
					tiles[std::make_pair(j, i)] = Tile((j * Tile::DEFAULT_SIZE), i * Tile::DEFAULT_SIZE);
				}
			}
		}
	}

	void Draw(sf::RenderWindow& window) {
		for (auto tile : tiles) {
			tile.second.Draw(window);
		}
	}

	void Update() {

	}

	//const std::vector<Tile>& getTiles() const {
	//	return tiles;
	//}

	void checkCollisions(Player& player) {
		const int f = 3.0f;
		auto auxPos = player.GetPos();
		auxPos.y -= player.GetHeight() / 2;

		int playerXIdx = std::round(auxPos.x) / Tile::DEFAULT_SIZE;
		int playerYIdx = std::round(auxPos.y) / Tile::DEFAULT_SIZE;
		
		std::cout << playerXIdx << " | " << playerYIdx << std::endl;

		sf::Vector2f newPosPlayer = player.GetPos();
		bool colisiona = false;

		if (tiles.find({playerXIdx - 1, playerYIdx}) != tiles.end()) {
			int tileXIdx = playerXIdx - 1;
			int tileYIdx = playerYIdx;
			auto tile = tiles[{tileXIdx, tileYIdx}];

			auto tileBounds = tile.GetGlobalBounds();

			auto pos = auxPos;
			pos.x -= (player.GetWidth() / f);

			colisiona = tileBounds.contains(pos);
			std::cout << (colisiona ? "SI" : "NO") << std::endl;

			if (colisiona) {
				auto tilePos = tile.GetPosition();
				newPosPlayer.x = tilePos.x + Tile::DEFAULT_SIZE + (player.GetWidth() / f);
			}
		}
		else if (tiles.find({ playerXIdx + 1, playerYIdx }) != tiles.end()) {
			int tileXIdx = playerXIdx + 1;
			int tileYIdx = playerYIdx;
			auto tile = tiles[{tileXIdx, tileYIdx}];

			auto tileBounds = tile.GetGlobalBounds();

			auto pos = auxPos;
			pos.x += (player.GetWidth() / f);

			colisiona = tileBounds.contains(pos);
			std::cout << (colisiona ? "SI" : "NO") << std::endl;

			if (colisiona) {
				auto tilePos = tile.GetPosition();
				newPosPlayer.x = tilePos.x - (player.GetWidth() / f);
			}
		}

		if (tiles.find({ playerXIdx, playerYIdx + 1}) != tiles.end()) {
			int tileXIdx = playerXIdx;
			int tileYIdx = playerYIdx + 1;
			auto tile = tiles[{tileXIdx, tileYIdx}];

			auto tileBounds = tile.GetGlobalBounds();

			//auto pos = player.GetPos();

			colisiona = tileBounds.intersects(player.GetGlobalBounds());
			//std::cout << (colisiona ? "SI" : "NO") << std::endl;

			if (colisiona) {
				auto tilePos = tile.GetPosition();
				newPosPlayer.y = tilePos.y;
			}
		}

		if(colisiona)
		player.SetPos(newPosPlayer);
	}

};

