#pragma once
#include <SFML/System/Time.hpp>
#include <SFML/Graphics.hpp>
#include <string>
#include <cmath>
#include <iostream>

class Game {

private:
    sf::RenderWindow window;
    sf::CircleShape player;
    sf::Clock clock;

    int width;
    int height;

    float player_vel = 0.f;
    float player_acc_default = 400.f;
    float player_deacc_default = 300.f;
    float player_max_vel = 400.f;
    int player_width = 50;

    void Initialize() {
        window.setFramerateLimit(60);
        window.setKeyRepeatEnabled(false);

        player = sf::CircleShape (player_width / 2);
        player.move(200.0f, 0);
        player.setFillColor(sf::Color::Green);
        player.setOrigin(player_width / 2, 0);
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
        float delta_time= 0.016f;

        sf::Vector2 movement(0.f, 0.f);
        

        float acceleration = 0;
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left)) {
            acceleration = -player_acc_default;
        }
        else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right)) {
            acceleration = player_acc_default;
        }
        else if (player_vel != 0) {
            if (player_vel > 0) acceleration = -player_deacc_default;
            else if(player_vel < 0) acceleration = player_deacc_default;
        }

        movement.x = (player_vel * delta_time) + ((pow(delta_time, 2) * acceleration) / 2);
        
        float temp_vel = player_vel + delta_time * acceleration;
        if ((player_vel < 0 && temp_vel > 0) || (player_vel > 0 && temp_vel < 0)) temp_vel = 0;

        if (temp_vel > player_max_vel) temp_vel = player_max_vel;
        else if (temp_vel < -player_max_vel) temp_vel = -player_max_vel;
        player_vel = temp_vel;

        player.move(movement);

        sf::Vector2f playerPos = player.getPosition();
        if (playerPos.x + (player_width / 2) > width) {
            player.setPosition(width - (player_width / 2), 0);
            player_vel = 0.f;
        }
        else if (playerPos.x - (player_width / 2) < 0) {
            player.setPosition((player_width / 2), 0);
            player_vel = 0.f;
        }

    }

    void Render() {
        window.clear();
        Draw();
        window.display();
    }

    void Draw() {
        //window.draw(player);
    }

public:
    Game(std::string name_, int width_ = 800, int height_ = 600) : window(sf::VideoMode(width_, height_), name_){
        width = width_;
        height = height_;
    }

    void Run() {
        Initialize();

        while (window.isOpen())
        {
            ProcessEvents();
            Update();
            Render();
            std::cout << player_vel << std::endl;
        }
    }

};