#include <iostream>
#include <string>
#include <SFML/Graphics.hpp>
#include <chrono>
#include <thread>
#include <Windows.h>


struct Pos { int x = 0; int y = 0; };

class Game {

private:
    Pos m_player_pos;
    int m_player_size = 25;
    int position_gap = 1;
    int m_width;
    int m_height;

    bool playerIsOutOfBoundaries() {
        return (m_player_pos.x < 0 || m_player_pos.x > (m_width - m_player_size * 2) || m_player_pos.y < 0 || m_player_pos.y > (m_height - m_player_size * 2) );
    }

public:
    Game(int width, int height): m_width(width) , m_height(height) {
    }

    int getPlayerSize() const {
        return m_player_size;
    }

    int getPlayerPositionX() const {
        return m_player_pos.x;
    }
    int getPlayerPositionY() const {
        return m_player_pos.y;
    }

    void MovePlayerUp() {
        m_player_pos.y -= position_gap;
        if (playerIsOutOfBoundaries()) m_player_pos.y += position_gap;
    }
    void MovePlayerDown() {
        m_player_pos.y += position_gap;
        if (playerIsOutOfBoundaries()) m_player_pos.y -= position_gap;
    }
    void MovePlayerRight() {
        m_player_pos.x += position_gap;
        if (playerIsOutOfBoundaries()) m_player_pos.x -= position_gap;
    }
    void MovePlayerLeft() {
        m_player_pos.x -= position_gap;
        if (playerIsOutOfBoundaries()) m_player_pos.x += position_gap;
    }
    
};

void updateGame(const sf::Event& event, Game& game) {
    //if (event.type == sf::Event::KeyPressed) {
    //    event.text.unicode
    //}

    /* KEYS */
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Up))
    {
        game.MovePlayerUp();
    }
    else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Down))
    {
        game.MovePlayerDown();
    }
    else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right))
    {
        game.MovePlayerRight();
    }
    else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left))
    {
        game.MovePlayerLeft();
    }

    //else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Escape))
    //{
    //    // quit...
    //}
    //else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Scan::Grave))
    //{
    //    // open in-game command line (if it's not already open)
    //}
};

void drawGame(sf::RenderWindow& window, const Game& game) {
    sf::CircleShape playerShape(game.getPlayerSize());
    playerShape.setFillColor(sf::Color::Green);


    playerShape.setPosition(game.getPlayerPositionX(), game.getPlayerPositionY());
    window.draw(playerShape);
};

int main()
{
    int width = 800;
    int height = 600;
    Game game(width, height);
    sf::RenderWindow window(sf::VideoMode(width, height), "RPG Game!");

    //sf::CircleShape shape(100.f);
    //shape.setFillColor(sf::Color::Green);
    int count = 0;
    sf::Clock clock; // starts the clock

    while (window.isOpen())
    {
        /*-------------Update------------*/
        sf::Event event;
        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
                window.close();

            updateGame(event, game);
        }
        /*-------------end Update------------*/

        //std::cout << count++ << std::endl;
        //std::cout << clock.getElapsedTime().asSeconds() << std::endl;
        std::cout << game.getPlayerPositionX() << " | " << game.getPlayerPositionY() << std::endl;
        //std::cout << count++ / clock.getElapsedTime().asSeconds() << std::endl;
        /*-------------Draw------------*/

        window.clear(sf::Color::Black);
        //window.draw(shape);
        //drawing here
        drawGame(window, game);

        window.display();

        //controla los fps
        /*-------------end Draw------------*/
        std::this_thread::sleep_for(std::chrono::milliseconds(10));
    }

    /*Ejemplo boludo de game loop*/

    //bool quit = false;
    //while (!quit) {
    //    cout << "Hola" << endl;

    //    if (GetKeyState(VK_ESCAPE) & 0x8000/*Check if high-order bit is set (1 << 15)*/)
    //    {
    //        quit = true;
    //    }

    //    std::this_thread::sleep_for(std::chrono::milliseconds(32));
    //}

    return 0;
}

