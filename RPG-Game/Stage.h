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
	int stage[18][32] = {
		{ 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
		{ 1,0,0,0,0,0,0,0,0,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
		{ 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
		{ 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
		{ 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
		{ 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
		{ 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
		{ 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
		{ 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
		{ 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
		{ 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
		{ 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
		{ 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
		{ 0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
		{ 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
		{ 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
		{ 1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1},
		{ 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
	};

	std::map<std::pair<int, int>,Tile> tiles;

public:
	Stage() {
		for (size_t i = 0; i < 18; i++)
		{
			for (size_t j = 0; j < 32; j++)
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
		auto posPlayer = player.GetPos();
		posPlayer.y -= player.GetHeight() / 2;

		int playerXIdx = std::round(posPlayer.x) / Tile::DEFAULT_SIZE;
		int playerYIdx = std::round(posPlayer.y) / Tile::DEFAULT_SIZE;
		
		std::cout << playerXIdx << " | " << playerYIdx << std::endl;

		if (tiles.find({playerXIdx - 1, playerYIdx}) != tiles.end()) {
			int tileXIdx = playerXIdx - 1;
			int tileYIdx = playerYIdx;
			auto tile = tiles[{tileXIdx, tileYIdx}];

			auto tileBounds = tile.GetGlobalBounds();

			auto pos = posPlayer;
			pos.x -= (player.GetWidth() / f);

			bool colisiona = tileBounds.contains(pos);
			std::cout << (colisiona ? "SI" : "NO") << std::endl;

			if (colisiona) {
				auto tilePos = tile.GetPosition();
				auto newPosPlayer = player.GetPos();
				newPosPlayer.x = tilePos.x + Tile::DEFAULT_SIZE + (player.GetWidth() / f);
				player.SetPos(newPosPlayer);
			}
		}
		else if (tiles.find({ playerXIdx + 1, playerYIdx }) != tiles.end()) {
			int tileXIdx = playerXIdx + 1;
			int tileYIdx = playerYIdx;
			auto tile = tiles[{tileXIdx, tileYIdx}];

			auto tileBounds = tile.GetGlobalBounds();

			auto pos = posPlayer;
			pos.x += (player.GetWidth() / f);

			bool colisiona = tileBounds.contains(pos);
			std::cout << (colisiona ? "SI" : "NO") << std::endl;

			if (colisiona) {
				auto tilePos = tile.GetPosition();
				auto newPosPlayer = player.GetPos();
				newPosPlayer.x = tilePos.x - (player.GetWidth() / f);
				player.SetPos(newPosPlayer);
			}
		}
		else if (tiles.find({ playerXIdx, playerYIdx + 1}) != tiles.end()) {
			int tileXIdx = playerXIdx;
			int tileYIdx = playerYIdx + 1;
			auto tile = tiles[{tileXIdx, tileYIdx}];

			auto tileBounds = tile.GetGlobalBounds();

			auto pos = posPlayer;
			pos.y += (player.GetWidth() / f);

			bool colisiona = tileBounds.contains(pos);
			std::cout << (colisiona ? "SI" : "NO") << std::endl;

			if (colisiona) {
				auto tilePos = tile.GetPosition();
				auto newPosPlayer = player.GetPos();
				newPosPlayer.y = tilePos.y;
				player.SetPos(newPosPlayer);
			}
		
		}
	}

};

