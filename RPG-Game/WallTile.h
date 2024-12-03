#pragma once
#include "Tile.h"

class WallTile : public Tile
{
public:
	WallTile(float posx = 0, float posy = 0) : Tile(posx, posy) {
		shape.setFillColor(fillcolor);
	}

	int Code() override { return CODE; };

	static const int CODE = 2;

private:
	sf::Color fillcolor = sf::Color::Blue;
};

