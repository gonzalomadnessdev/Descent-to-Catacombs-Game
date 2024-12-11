#pragma once
#include <SFML/Graphics.hpp>
#include "Character.h"
#include "Player.h"
#include <iostream>

class FinalBoss : public Character
{
private:
	sf::Sprite mace_sprite;
	sf::Texture mace_texture;

	sf::IntRect pos_walk;
	sf::IntRect pos_attk;

	Player* player;

	float min_move_distance = 100;
	float min_attk_distance = 400;

	sf::Vector2f maceHiddenPos = { -100000.f, -100000.f };

	bool is_attacking = false;

	bool to_right = true;

	int tick_100_ms = 0;
	int tick_500_ms = 0;
	int tick_s = 0;

	float vel = 1.2f;

	sf::Vector2f prevPos;


public:
	inline static const int CHARACTER_CODE = 10;
	FinalBoss(Player* _player) : player(_player) , Character("./img/boss_spritesheet.png", 62, 86, 3000, 20, 100)
	{
		pos_walk = sf::IntRect(((width) * 1), 0, width, height);
		pos_attk = sf::IntRect(((width) * 2), 0, width, height);

		sf::Image maceImage;
		maceImage.loadFromFile("./img/mace.png");
		maceImage.createMaskFromColor(sf::Color::Red);

		mace_texture.loadFromImage(maceImage);
		mace_sprite.setTexture(mace_texture);

		float maceXOrigin = -(width / 2.5f);
		float maceYOrigin = (mace_sprite.getGlobalBounds().height / 2);
		mace_sprite.setOrigin(maceXOrigin, maceYOrigin);
		mace_sprite.setPosition(maceHiddenPos);
	}

	void Update() override {

		tick_100_ms++;
		tick_100_ms = tick_100_ms % 6; // 100ms

		tick_500_ms++;
		tick_500_ms = tick_500_ms % 30; // 100ms

		tick_s++;
		tick_s = tick_s % 60; // 1s
		

		sf::Vector2f dist = sprite.getPosition() - player->GetPos();

		//std::cout << "distancia x entre boss y player: " << dist.x << std::endl;

		if (dist.x >= 0) {
			sprite.setScale({ -1,1 });
			to_right = false;
		}
		else {
			sprite.setScale({ 1,1 });
			to_right = true;
		}

		if (tick_100_ms == 0) {
			if (sprite.getPosition() != prevPos) {
				if (pos_current == pos_idle) pos_current = pos_walk;
				else pos_current = pos_idle;
			}

			prevPos = sprite.getPosition();
		}


		if (tick_500_ms == 0) {
			if (std::abs(dist.x) < min_attk_distance && (std::rand() % 3 == 0)) { //%33.34 posibilidades de ser atacado
				pos_current = pos_attk;
			}
		}

		if (pos_current == pos_attk) {

			auto pos = sprite.getPosition();
			pos.y += 62 - height; // a  ojo


			if (to_right) {
				mace_sprite.setRotation(0);
			}
			else {
				mace_sprite.setRotation(180);
			}
			mace_sprite.setPosition(pos);
		}
		else {
			mace_sprite.setPosition(maceHiddenPos);
		}


		texture.loadFromImage(image, pos_current);
		sprite.setTexture(texture);

		//player dmg
		if (player->GetGlobalBounds().intersects(mace_sprite.getGlobalBounds())) {
			player->takeDamage(damage);
		}

		//move
		if (!is_attacking && std::abs(dist.x) > min_move_distance) {
			
			if (dist.x > 0) {
				sprite.move({ -vel, 0 });
			}
			else {
				sprite.move({ vel, 0 });
			}
		}
		std::cout << is_attacking << " / " << (pos_current == pos_attk) << std::endl;
		//healthbar
		healthBar->SetHealth(health);
		healthBar->SetPosition(sprite.getPosition());
		healthBar->Update();
	}

	void Draw(sf::RenderWindow& window) {
		Character::Draw(window);
		window.draw(mace_sprite);
	};
};

