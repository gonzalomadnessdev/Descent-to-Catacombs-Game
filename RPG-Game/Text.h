#pragma once
#include <SFML/Graphics.hpp>
#include "AbstractEntity.h"
#include <string>

class Text : public AbstractEntity
{
public:
	Text(int size, sf::Color color, std::string fontName) {

		if (!font.loadFromFile("./fonts/" + fontName +".ttf")) {
			// Handle error if font is not loaded
			exit(-1);
		}

		// select the font
		text.setFont(font); // font is a sf::Font

		// set the string to display
		text.setString("");

		// set the character size
		text.setCharacterSize(size); // in pixels, not points!

		// set the color
		text.setFillColor(color);
	}

	Text(int size, sf::Color color) : Text(size, color, "pirata")
	{
	}

	void Draw(sf::RenderWindow& window) override {
		window.draw(text);
	}

	void Update() override {

	}

	void SetString(std::string str) {
		text.setString(str);
	}

	void SetPos(float x, float y) {
		text.setPosition(x, y);
	}

	void CenterOrigin() {
		auto bounds = text.getGlobalBounds();
		text.setOrigin(bounds.width / 2, bounds.height / 2);
	}
	

private:
	sf::Text text;
	sf::Font font;
};

