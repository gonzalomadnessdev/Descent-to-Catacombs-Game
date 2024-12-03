#include "Player.h"
#include <SFML/Graphics.hpp>
#include <string>
#include <cmath>
#include <iostream>

Player::Player() {
	image.loadFromFile("./img/siegward_spritesheet.png");
	image.createMaskFromColor(sf::Color::Red);

	sf::Image swordImage;
	swordImage.loadFromFile("./img/sword.png");
	swordImage.createMaskFromColor(sf::Color::Red);

	sword_texture.loadFromImage(swordImage);
	sword_sprite.setTexture(sword_texture);

	sword_sprite.setOrigin(-(width / 2.5f), (sword_sprite.getLocalBounds().height / 2));

	texture.loadFromImage(image, sf::IntRect(0, 0, width, height));
	sprite.setTexture(texture);

	sprite.setOrigin(width / 2, height);
	sprite.setScale({1,1});

	pos_idle = sf::IntRect(((width) * 0), 0, width, height);
	pos_walk = sf::IntRect(((width) * 1), 0, width, height);
	pos_attk = sf::IntRect(((width) * 2), 0, width, height);
	pos_current = pos_idle;

	prevPos = GetPos();
}

sf::Vector2f Player::GetPos() {
	return sprite.getPosition();
}

void Player::SetPos(sf::Vector2<float> pos) {
	sprite.setPosition(pos);
}

void Player::SetVel(float vel) {
	this->vel = vel;
}

void Player::Draw(sf::RenderWindow& window) {
	window.draw(sprite);
	window.draw(sword_sprite);

}

bool Player::isLookingToRight(){
	return to_right;
}

bool Player::isLookingToLeft() {
	return !to_right;
}

void Player::ApplyGravity() {
	auto pos = GetPos();
	pos.y += 6;
	SetPos(pos);

	if(vel_up < 0) vel_up += 10;
}

void Player::Update() {
	//if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right)) {
	//	
	//}
	//else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left)) {
	//	to_right = false;
	//}
	prevPos = GetPos();

	//std::cout << is_falling << std::endl;
	

	is_attacking = false;
	tick_changepos++;
	tick_changepos = tick_changepos % 10; // 
	float delta_time = 0.016f;

	sf::Vector2 movement(0.f, 0.f);
	float acceleration = 0;

	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Space) || sf::Keyboard::isKeyPressed(sf::Keyboard::Left) || sf::Keyboard::isKeyPressed(sf::Keyboard::Right)) {

		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Space)) {
			if (!is_falling) {
				is_attacking = true;
				pos_current = pos_attk;
				texture.loadFromImage(image, pos_current);
				sprite.setTexture(texture);
				vel = 0.f;
			}
		}
		else {
			if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left)) {
				if (!is_attacking && !is_falling) {
					to_right = false;
					acceleration = -acc_default;
					if (tick_changepos == 0) {
						if (pos_current == pos_idle) pos_current = pos_walk;
						else pos_current = pos_idle;
					}

					texture.loadFromImage(image, pos_current);
					sprite.setTexture(texture);
					sprite.setScale({ -1, 1 });
					if (vel > 0) acceleration = -deacc_default * 10;
				}
			}
			else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right)) {
				if (!is_attacking && !is_falling) {
					to_right = true;
					acceleration = acc_default;
					if (tick_changepos == 0) {
						if (pos_current == pos_idle) pos_current = pos_walk;
						else pos_current = pos_idle;
					}
					texture.loadFromImage(image, pos_current);
					sprite.setTexture(texture);
					sprite.setScale({ 1, 1 });
					if (vel < 0) acceleration = deacc_default * 10;
				}
			}
		}
	}
	else {
		if (vel != 0) {
			if (vel > 0) acceleration = -deacc_default;
			else if (vel < 0) acceleration = deacc_default;
		}
		pos_current = pos_idle;
		texture.loadFromImage(image, pos_current);
		sprite.setTexture(texture);
	}

	//aca?
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Up) && !is_falling && !is_attacking) {
		//std::cout << "salta" << std::endl;
		if (vel_up >= 0) vel_up = max_vel_up;
	}


	//opcion de manejo de la espadita A
	//if (is_attacking) {
	//	auto pos = sprite.getPosition();
	//	pos.y += 42; // a  ojo

	//	if (isLookingToLeft()) {
	//		sword_sprite.setRotation(180);
	//	}
	//	else {
	//		pos.y -= 1; // a  ojo
	//		sword_sprite.setRotation(0);
	//	}
	//	sword_sprite.setPosition(pos);
	//}
	//else {
	//	sword_sprite.setPosition(-100000, -100000);
	//}

	//opcion de manejo de la espadita A
	if (is_attacking && !is_falling) {
		if (pos_current == pos_attk) {
			auto pos = sprite.getPosition();
			pos.y += 42 - height; // a  ojo


			if (isLookingToLeft()) {
				sword_sprite.setRotation(180);
			}
			else {
				pos.y -= 1; // a  ojo
				sword_sprite.setRotation(0);
			}
			sword_sprite.setPosition(pos);
		}
	}
	else {
		sword_sprite.setPosition(-100000, -100000);
	}
	
	movement.x = (vel * delta_time) + ((pow(delta_time, 2) * acceleration) / 2);

	float temp_vel = vel + delta_time * acceleration;
	if ((vel < 0 && temp_vel > 0) || (vel > 0 && temp_vel < 0)) temp_vel = 0.0f;

	if (temp_vel > max_vel) temp_vel = max_vel;
	else if (temp_vel < -max_vel) temp_vel = -max_vel;
	vel = temp_vel;

	//up
	if (vel_up > 0) vel_up = 0;
	
	movement.y = (vel_up * delta_time);

	//end up

	sprite.move(movement);
}