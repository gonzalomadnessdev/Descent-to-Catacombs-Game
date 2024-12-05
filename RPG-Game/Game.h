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
	//sf::Clock clock;

	//std::vector<AbstractEntity*> entities;

	Player siegward;
	Stage stage;
	std::vector<Enemy*> enemies;

	//Text* healthBar;
	HealthBar* healthBar;

	void Initialize() {

		healthBar = new HealthBar(siegward.GetPos(), 100, 5, siegward.getHealth());
		healthBar->SetOrigin({ 50, 10.f + siegward.GetHeight() });

		window.setFramerateLimit(60);
		window.setKeyRepeatEnabled(false);

		//stage
		//entities.push_back(&stage);

		stage.LoadEnemies(enemies);
		siegward.SetPos({80,80});




		//for (auto enemy : enemies) {
		//	entities.push_back(enemy);
		//}

		////entities.push_back(&siegward);

		//sort(entities.begin(), entities.end(), [](AbstractEntity* e1, AbstractEntity* e2) { return e1->GetDrawOrder() < e2->GetDrawOrder();});
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

		siegward.ApplyGravity();
		siegward.checkFallingState(stage.GetTiles());
		stage.checkCollisions(siegward);
		
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
		if (siegward.GetPos().y > height * 1.5) {
			siegward.Kill();
		}

		//for (auto entity : entities) {
		//	entity->Update();
		//}
		siegward.Update();
		for (auto enemy : enemies) {
			enemy->Update();
		}

		stage.Update();

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
		stage.Draw(window);
		for (auto enemy : enemies) {
			enemy->Draw(window);
		}
		siegward.Draw(window);

		healthBar->Draw(window);

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