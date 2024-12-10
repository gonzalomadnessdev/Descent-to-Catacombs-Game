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
#include "Enemy.h"
#include "Text.h"
#include "HealthBar.h"

class Game {

private:
	sf::RenderWindow window;

	int current_stage = 1;
	int last_stage = 2;

	sf::Vector2f player_initial_pos = { 80,0 };

	Player siegward;
	Stage* stage;
	std::vector<Enemy*> enemies;

	HealthBar* healthBar;

	Text* stageText;

	Enemy* finalBoss = nullptr;

	void Initialize() {

		stageText = new Text(20, sf::Color::White);
		stageText->SetPos(10, 10);

		healthBar = new HealthBar(siegward.GetPos(), 100, 5, siegward.getHealth());
		healthBar->SetOrigin({ 50, 10.f + siegward.GetHeight() });

		window.setFramerateLimit(60);
		window.setKeyRepeatEnabled(false);

		stage = new Stage(current_stage);
		stage->LoadEnemies(enemies);
		siegward.SetPos(player_initial_pos);
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
		stageText->SetString("Stage : " + std::to_string(current_stage));
		sf::Vector2f playerPos = siegward.GetPos();

		if (playerPos.x > width) {
			siegward.SetPos(sf::Vector2f(width, playerPos.y));
		}
		else if (playerPos.x < 0) {
			siegward.SetPos(sf::Vector2f(0, playerPos.y));
		}

		siegward.ApplyGravity();
		siegward.checkFallingState(stage->GetTiles());
		stage->checkCollisions(siegward);
		
		bool encontroEnemigo = false;
		std::vector<Enemy*>::iterator itEnemyHitted;
		for (auto it = enemies.begin(); it != enemies.end();++it) {
			if (siegward.isHittedBySword((*it)->GetGlobalBounds())) {
				//std::cout << "lo golpea" << std::endl;
				itEnemyHitted = it;
				encontroEnemigo = true;
				break;
			}	
		}
		if (encontroEnemigo) {
			(*itEnemyHitted)->takeDamage(siegward.getDamage());
			std::cout << "enemy health: " << (*itEnemyHitted)->getHealth() << std::endl;
			if (!(*itEnemyHitted)->isAlive()) {
				enemies.erase(itEnemyHitted);
			}
		}

		bool enemigoTocaPlayer = false;
		std::vector<Enemy*>::iterator itEnemyHitter;
		for (auto it = enemies.begin(); it != enemies.end();++it) {

			if ((*it)->GetGlobalBounds().contains(siegward.GetPosCenter())) {
				std::cout << "me tocan" << std::endl;
				itEnemyHitter = it;
				enemigoTocaPlayer = true;
				break;
			}
		}

		if (enemigoTocaPlayer) {
			int dmg = (*itEnemyHitter)->getDamage();
			siegward.takeDamage(dmg);
			std::cout << "siegward health: " << siegward.getHealth() << std::endl;
		}



		//std::cout << siegward.GetPos().x << " | " << siegward.GetPos().y << std::endl;
		if (siegward.GetPos().y > height * 1.2) {
			if (current_stage < last_stage) {
				current_stage++;
				
				delete stage;
				stage = nullptr;

				enemies.clear();
				stage = new Stage(current_stage);
				stage->LoadEnemies(enemies);
				auto pos = siegward.GetPos();
				pos.y = 0;
				siegward.SetPos(pos);

				if (current_stage == last_stage) {
					finalBoss =enemies[0];
				}
			}
			else {
				siegward.Kill();
			}
			
		}

		//for (auto entity : entities) {
		//	entity->Update();
		//}
		siegward.Update();
		for (auto enemy : enemies) {
			enemy->Update();
		}

		stage->Update();

		healthBar->SetHealth(siegward.getHealth());
		healthBar->SetPosition(siegward.GetPos());
		healthBar->Update();
		//healthBar->SetString("Health: " + std::to_string(siegward.getHealth()));
		//std::cout << siegward.isFalling() << std::endl;
	}

	void Render() {
		window.clear();
		Draw();
		window.display();
	}

	void Draw() {

		
		//for (auto entity : entities) {
		//	entity->Draw(window);
		//}
		stage->Draw(window);
		

		for (auto enemy : enemies) {
			enemy->Draw(window);
		}
		siegward.Draw(window);

		healthBar->Draw(window);

		stageText->Draw(window);
		//game over region
		if (!siegward.isAlive()) {
			//mover todo esto a una clase texto
			
			Text gameoverTxt(40, sf::Color::White);
			gameoverTxt.SetString("Game Over");
			gameoverTxt.CenterOrigin();
			gameoverTxt.SetPos((width / 2), (height / 2));


			//auto bounds = text.getGlobalBounds();
			//text.setPosition;

			gameoverTxt.Draw(window);
		}


		//end game over region

		//win region
		if (finalBoss != nullptr && !finalBoss->isAlive()) {
			Text winTxt(40, sf::Color::White);
			winTxt.SetString("You've killed the menace in the catacombs. Congratulations!");
			winTxt.CenterOrigin();
			winTxt.SetPos((width / 2), (height / 2));


			//auto bounds = text.getGlobalBounds();
			//text.setPosition;

			winTxt.Draw(window);
		}
		//end win region
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

			if (siegward.isAlive() && (finalBoss == nullptr || finalBoss->isAlive())) {
				Update();
			}

			Render();
		}

		
	}

};