#pragma once
#include <SFML/System/Time.hpp>
#include <SFML/Graphics.hpp>
#include <string>
#include <cmath>
#include <iostream>
#include "Player.h"
#include "Stage.h"
#include <vector>
#include "AbstractEntity.h"

class Game {

private:
	sf::RenderWindow window;
	//sf::Clock clock;

	std::vector<AbstractEntity*> entities;

	Player siegward;
	Stage stage;

	void Initialize() {
		window.setFramerateLimit(60);
		window.setKeyRepeatEnabled(false);

		//stage
		entities.push_back(&stage);

		//player initialization and save
		sf::Vector2f initialPosPlayer;
		initialPosPlayer.x = 80;
		initialPosPlayer.y = 80;
		siegward.SetPos(initialPosPlayer);

		entities.push_back(&siegward);



	}

	void ProcessEvents() {
		sf::Event event;
		while (window.pollEvent(event))
		{
			if (event.type == sf::Event::Closed)
				window.close();
		}
	}

	void Update() {
		sf::Vector2f playerPos = siegward.GetPos();

		if (playerPos.x > width) {
			siegward.SetPos(sf::Vector2f(width, playerPos.y));
		}
		else if (playerPos.x < 0) {
			siegward.SetPos(sf::Vector2f(0, playerPos.y));
		}

		//efecto grav
		siegward.ApplyGravity();
		//end 
		stage.checkCollisions(siegward);

		std::cout << siegward.GetPos().x << " | " << siegward.GetPos().y << std::endl;
		if (siegward.GetPos().y > height * 1.5) {
			siegward.Kill();
		}

		for (auto entity : entities) {
			entity->Update();
		}
	}

	void Render() {
		window.clear();
		Draw();
		window.display();
	}

	void Draw() {

		for (auto entity : entities) {
			entity->Draw(window);
		}

		//game over region
		if (!siegward.isAlive()) {
			//mover todo esto a una clase texto

			sf::Text text;
			sf::Font font;
			if (!font.loadFromFile("./fonts/pirata.ttf")) {
				// Handle error if font is not loaded
				exit(-1);
			}
			// select the font
			text.setFont(font); // font is a sf::Font

			// set the string to display
			text.setString("Game Over");

			// set the character size
			text.setCharacterSize(40); // in pixels, not points!

			// set the color
			text.setFillColor(sf::Color::White);

			auto bounds = text.getGlobalBounds();
			text.setPosition((width / 2) - (bounds.width / 2), (height / 2) - (bounds.height / 2));

			window.draw(text);
		}
		//end game over region
	}

public:

	int width;
	int height;

	Game(std::string name_, int width_ = 1280, int height_ = 720) : window(sf::VideoMode(width_, height_), name_) {
		width = width_;
		height = height_;
	}

	void Run() {
		Initialize();

		while (window.isOpen())
		{
			ProcessEvents();

			if (siegward.isAlive()) {
				Update();
			}

			Render();
		}

		
	}

};