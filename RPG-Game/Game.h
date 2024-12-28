#pragma once
#include <SFML/System/Time.hpp>
#include <SFML/Graphics.hpp>
#include <string>
#include <cmath>
#include <iostream>
#include <algorithm>
#include "Player.h"
#include "Stage.h"
#include <vector>
#include <sstream>
#include <iomanip>
#include "AbstractEntity.h"
#include "Enemy.h"
#include "Text.h"
#include "HealthBar.h"

struct ScoreRecord {
	char name[20];
	long points;
};

class Game {
	
private:
	bool isGameOver = true;
	inline static const int STATE__MENU = 0;
	inline static const int STATE__MENU_CONTROLS = 1;
	inline static const int STATE__MENU_ADD_NAME = 2;
	inline static const int STATE__MENU_SCORES = 3;
	inline static const int STATE__GAME_RUNNING = 10;

	int gameState = STATE__MENU;

	sf::RenderWindow window;

	int points = 0;
	int current_stage = 1;
	int last_stage = 3;

	sf::Vector2f player_initial_pos = { 80,0 };

	Player* siegward;
	Stage* stage;
	std::vector<Character*> enemies;

	HealthBar* healthBar;

	Text* stageText;
	Text* pointsText;

	Text* mainMenuTitle = new Text(80, sf::Color::White);
	Text* mainMenuNav = new Text(30, sf::Color::White);

	Text* controlsMenuText = new Text(30, sf::Color::White, "consolas");

	Text* menuNavText = new Text(30, sf::Color::White);
	Text* menuNavText2 = new Text(30, sf::Color::White);
	Text* menuNavText3 = new Text(30, sf::Color::White);

	Text* scoresText = new Text(22, sf::Color::White, "consolas");

	Text* menuPlayerName = new Text(32, sf::Color::White);
	Text* playerNameText = new Text(24, sf::Color::White, "consolas");

	Character* finalBoss = nullptr;

	std::string gameName = "Descent to Catacombs";
	std::string navMainMenuStr = "NEW GAME\nCONTROLS\nSCORES";

	std::string controlsStr = "Controls: \n \nLeft Arrow (Left)\nRight Arrow (Right)\nUp Arrow (Jump)\nSPACE (Attack)";
	std::string navMenu = "Esc to Back";
	std::string navMenu2 = "Enter to Start\nEsc to Back";
	std::string navMenu3 = "Enter to Main Menu";

	std::string scoreString = "";

	int mainMenuCursorSelected = 0;
	int mainMenuCursorSelectedMax = 3;

	sf::CircleShape *cursorMainMenu;
	sf::Vector2f cursorInitialPos;

	std::string defaultPlayerName = "SIEGWARD";
	std::string playerName = defaultPlayerName;

	void UpdateScore() {
		std::vector<ScoreRecord> scores;

		std::ifstream filein("data/score.dat", std::ios::binary);

		ScoreRecord scoreTemp;
		while (filein.read(reinterpret_cast<char*>(&scoreTemp), sizeof(ScoreRecord))) {
			for (auto& c : scoreTemp.name) c = toupper(c);
			scores.push_back(scoreTemp);
		}
		filein.close();
		
		std::string _playerName = playerName;
		auto mismoPlayerScoreIt = std::find_if(scores.begin(), scores.end(), [_playerName](ScoreRecord sr) { return sr.name == _playerName;});

		if (mismoPlayerScoreIt != scores.end()) {
			if (mismoPlayerScoreIt->points >= points) {
				return;
			}
			else {
				mismoPlayerScoreIt->points = points;
			}
		}
		else {
			ScoreRecord sr;
			strcpy_s(sr.name, playerName.c_str());
			sr.points = points;
			scores.push_back(sr);
		}
		
		std::sort(scores.begin(), scores.end(), [](ScoreRecord sr1, ScoreRecord sr2) { return sr1.points > sr2.points; });
		scores.pop_back();

		std::ofstream fileout("data/score.dat", std::ios::binary);

		for (auto& score : scores) {
			fileout.write(reinterpret_cast<char*>(&score), sizeof(score));
		}

		fileout.close();
	}

	void LoadScores() {
		std::ifstream file("data/score.dat", std::ios::binary);

		std::stringstream ss;

		ScoreRecord scoreTemp;
		while (file.read(reinterpret_cast<char*>(&scoreTemp), sizeof(ScoreRecord))) {
			//scoreString += (std::string)scoreTemp.name + std::to_string(scoreTemp.points) + "\n";
			for (auto& c : scoreTemp.name) c = toupper(c);
			ss << std::left << std::setw(10) << std::setfill('.') << scoreTemp.name << std::right << std::setw(10) << scoreTemp.points << std::endl;
		}
		

		file.close();

		scoreString = ss.str();
		std::cout << scoreString;
		scoresText->SetString(scoreString);
		scoresText->SetPos((width / 2), (height / 2));
		scoresText->CenterOrigin();
	}
	
	void LoadMenu() {

		if (stage != nullptr) {
			delete stage;
			stage = nullptr;
		}

		stage = new Stage(-1);
		cursorMainMenu = new sf::CircleShape(7);
		cursorMainMenu->setFillColor(sf::Color::White);

		scoreString = "";

		cursorInitialPos = sf::Vector2f(550, 380);
		cursorMainMenu->setPosition(cursorInitialPos);

		mainMenuTitle->SetString(gameName);
		mainMenuTitle->SetPos((width / 2), (height / 3));
		mainMenuTitle->CenterOrigin();

		mainMenuNav->SetString(navMainMenuStr);
		mainMenuNav->SetPos((width / 2), height - 300);
		mainMenuNav->CenterOrigin();

		controlsMenuText->SetString(controlsStr);
		controlsMenuText->SetPos((width / 2), (height / 3));
		controlsMenuText->CenterOrigin();

		menuNavText->SetString(navMenu);
		menuNavText->SetPos((width / 2), (height - 40));
		menuNavText->CenterOrigin();
		menuNavText->CenterOrigin();

		menuNavText2->SetString(navMenu2);
		menuNavText2->SetPos((width / 2), (height - 70));
		menuNavText2->CenterOrigin();
		menuNavText2->CenterOrigin();

		menuNavText3->SetString(navMenu3);
		menuNavText3->SetPos((width / 2), (height - 40));
		menuNavText3->CenterOrigin();
		menuNavText3->CenterOrigin();

		menuPlayerName->SetString("Player name: ");
		menuPlayerName->SetPos(550, (height / 2));
		menuPlayerName->CenterOrigin();

		playerNameText->SetString(playerName);
		playerNameText->SetPos(700, (height / 2) - 5);
		playerNameText->CenterOrigin();

	}

	void InitializeGame() {

		current_stage = 1;
		points = 0;

		//ensure clear
		for (auto& enemy : enemies) {
			if (enemy != nullptr) {
				delete enemy;
				enemy = nullptr;
			}
		}
		enemies.clear();

		if (finalBoss != nullptr) {
			finalBoss = nullptr;
		}

		if (siegward != nullptr) {
			delete siegward;
			siegward = nullptr;
		}

		if (stage != nullptr) {
			delete stage;
			stage = nullptr;
		}

		//end ensure clear

		isGameOver = false;
		siegward = new Player();

		stageText = new Text(25, sf::Color::White);
		stageText->SetPos(10, height - 40);


		pointsText = new Text(25, sf::Color::White);
		pointsText->SetPos(100, height - 40);

		healthBar = new HealthBar(siegward->GetPos(), 100, 5, siegward->getHealth());
		healthBar->SetOrigin({ 50, 10.f + siegward->GetHeight() });

		window.setFramerateLimit(60);
		window.setKeyRepeatEnabled(false);

		if (stage != nullptr) {
			delete stage;
			stage = nullptr;
		}

		stage = new Stage(current_stage);
		stage->LoadEnemies(enemies, siegward);
		siegward->SetPos(player_initial_pos);
	}

	void ProcessEvents() {
		sf::Event event;
		while (window.pollEvent(event))
		{
			if (event.type == sf::Event::Closed)
				window.close();
			if (event.type == sf::Event::KeyPressed) {

				if (gameState == STATE__MENU) {
					if (event.key.code == sf::Keyboard::Down) {
						mainMenuCursorSelected++;
						if (mainMenuCursorSelected == mainMenuCursorSelectedMax) mainMenuCursorSelected = 0;
					}
					else if (event.key.code == sf::Keyboard::Up) {
						mainMenuCursorSelected--;
						if (mainMenuCursorSelected < 0) mainMenuCursorSelected = (mainMenuCursorSelectedMax - 1);
					}
					else if (event.key.code == (sf::Keyboard::Enter)) {

						switch (mainMenuCursorSelected)
						{
						case 0:
							//gameState = STATE__GAME_RUNNING;
							//InitializeGame();
							gameState = STATE__MENU_ADD_NAME;
							break;
						case 1:
							gameState = STATE__MENU_CONTROLS;
							break;
						case 2:
							gameState = STATE__MENU_SCORES;
							break;
						default:
							break;
						}
					}

				}
				else if (gameState == STATE__MENU_ADD_NAME) {
					if (event.key.code == (sf::Keyboard::Escape)) {
						gameState = STATE__MENU;
					}else if (event.key.code == (sf::Keyboard::Enter) && !playerName.empty()) {
						gameState = STATE__GAME_RUNNING;
						InitializeGame();
					}else if (event.key.code == sf::Keyboard::BackSpace) {
						if (!playerName.empty()) playerName.pop_back();
					}
					else if (event.key.code >= sf::Keyboard::A && event.key.code <= sf::Keyboard::Z) {
						playerName += static_cast<char>(event.key.code - sf::Keyboard::A + 'A');;
					}
				}
				else if (gameState == STATE__MENU_CONTROLS) {
					if (event.key.code == sf::Keyboard::Escape) {
						gameState = STATE__MENU;
					}
				}
				else if (gameState == STATE__MENU_SCORES) {
					if (event.key.code == (sf::Keyboard::Escape)) {
						gameState = STATE__MENU;
					}
				}
				else if (gameState == STATE__GAME_RUNNING) {
					if (isGameOver && event.key.code == (sf::Keyboard::Enter)) {
						LoadMenu();
						gameState = STATE__MENU;
					}
				}

			}

		}
	}

	void UpdateGame() {
		if (isGameOver) { return; };

		sf::Vector2f playerPos = siegward->GetPos();

		if (playerPos.x > width) {
			siegward->SetPos(sf::Vector2f(width, playerPos.y));
		}
		else if (playerPos.x < 0) {
			siegward->SetPos(sf::Vector2f(0, playerPos.y));
		}

		siegward->ApplyGravity();
		siegward->checkFallingState(stage->GetTiles());
		stage->checkCollisions(siegward);

		bool encontroEnemigo = false;
		std::vector<Character*>::iterator itEnemyHitted;
		for (auto it = enemies.begin(); it != enemies.end();++it) {
			if (siegward->isHittedBySword((*it)->GetGlobalBounds())) {
				itEnemyHitted = it;
				encontroEnemigo = true;
				break;
			}
		}
		if (encontroEnemigo) {
			(*itEnemyHitted)->takeDamage(siegward->getDamage());
			if (!(*itEnemyHitted)->isAlive()) {
				points += (300 * current_stage * (*itEnemyHitted)->getDamage()) / 10;
				enemies.erase(itEnemyHitted);
			}
		}

		bool enemigoTocaPlayer = false;
		std::vector<Character*>::iterator itEnemyHitter;
		for (auto it = enemies.begin(); it != enemies.end();++it) {

			if ((*it)->GetGlobalBounds().contains(siegward->GetPosCenter())) {
				itEnemyHitter = it;
				enemigoTocaPlayer = true;
				break;
			}
		}

		if (enemigoTocaPlayer) {
			int dmg = (*itEnemyHitter)->getDamage();
			siegward->takeDamage(dmg);
		}

		if (siegward->GetPos().y > height * 1.2) {
			if (current_stage < last_stage) {
				current_stage++;

				delete stage;
				stage = nullptr;

				enemies.clear();
				stage = new Stage(current_stage);
				stage->LoadEnemies(enemies, siegward);
				auto pos = siegward->GetPos();
				pos.y = 0;
				siegward->SetPos(pos);

				if (current_stage == last_stage) {
					finalBoss = enemies[0];
				}
			}
			else {
				siegward->Kill();
			}

		}

		siegward->Update();
		for (auto enemy : enemies) {
			enemy->Update();
		}

		stage->Update();

		healthBar->SetHealth(siegward->getHealth());
		healthBar->SetPosition(siegward->GetPos());
		healthBar->Update();

		stageText->SetString("Stage : " + std::to_string(current_stage));
		pointsText->SetString("Points : " + std::to_string(points));

		if (!siegward->isAlive() || (finalBoss != nullptr && !finalBoss->isAlive())) {
			UpdateScore();
			isGameOver = true;
		};
	}

	void Update() {
		if (gameState == STATE__GAME_RUNNING) {
			if (!isGameOver) {
				UpdateGame();
			}
		}
		else if (gameState == STATE__MENU) {
			auto newPos = cursorInitialPos;
			newPos.y = newPos.y + mainMenuCursorSelected * 38;
			cursorMainMenu->setPosition(newPos);


		}
		else if (gameState == STATE__MENU_SCORES) {
			if (scoreString.empty()) {
				LoadScores();
			}
		}
		else if (gameState == STATE__MENU_ADD_NAME) {
			playerNameText->SetString(playerName);
		}

	}

	void Render() {
		window.clear();
		Draw();
		window.display();
	}

	void DrawGame() {

		stage->Draw(window);


		for (auto enemy : enemies) {
			enemy->Draw(window);
		}
		siegward->Draw(window);

		healthBar->Draw(window);

		stageText->Draw(window);
		pointsText->Draw(window);

		//game over region
		if (isGameOver) {
			if (finalBoss != nullptr && !finalBoss->isAlive()) {
				Text winTxt(40, sf::Color::White);
				winTxt.SetString("You've killed the menace in the catacombs. Congratulations!");
				winTxt.CenterOrigin();
				winTxt.SetPos((width / 2), (height / 2));
				//auto bounds = text.getGlobalBounds();
				//text.setPosition;

				winTxt.Draw(window);
			}
			else {
				//mover todo esto a una clase texto

				Text gameoverTxt(40, sf::Color::White);
				gameoverTxt.SetString("Game Over");
				gameoverTxt.CenterOrigin();
				gameoverTxt.SetPos((width / 2), (height / 2));

				//auto bounds = text.getGlobalBounds();
				//text.setPosition;

				gameoverTxt.Draw(window);
			}
		}
		if (!siegward->isAlive()) {

		}
	}

	void Draw() {
		if (gameState == STATE__GAME_RUNNING) {
			DrawGame();
			if (isGameOver) {
				menuNavText3->Draw(window);
			}
		}
		else if (gameState == STATE__MENU) {
			stage->Draw(window);
			mainMenuTitle->Draw(window);
			mainMenuNav->Draw(window);
			window.draw(*cursorMainMenu);
		}
		else if (gameState == STATE__MENU_CONTROLS) {
			stage->Draw(window);
			controlsMenuText->Draw(window);
			menuNavText->Draw(window);
		}
		else if (gameState == STATE__MENU_SCORES) {
			stage->Draw(window);
			scoresText->Draw(window);
			menuNavText->Draw(window);
		}
		else if (gameState == STATE__MENU_ADD_NAME) {
			stage->Draw(window);
			playerNameText->Draw(window);
			menuPlayerName->Draw(window);
			menuNavText2->Draw(window);
		}
	}

public:

	int width;
	int height;

	Game(std::string name_, int width_ = 1280, int height_ = 720) : window(sf::VideoMode(width_, height_), name_) {
		width = width_;
		height = height_;
	}

	void Run() {
		LoadMenu();

		while (window.isOpen())
		{
			ProcessEvents();

			Update();

			Render();
		}
	}

};