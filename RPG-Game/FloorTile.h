#pragma once
#include "Tile.h"

class FloorTile : public Tile
{
public:
	FloorTile(float posx = 0, float posy = 0) : Tile(posx, posy) {
		shape.setFillColor(fillcolor);
	}

	int Code() override { return CODE; };

	static const int CODE = 1;

private:
	sf::Color fillcolor = sf::Color::Red;
};

