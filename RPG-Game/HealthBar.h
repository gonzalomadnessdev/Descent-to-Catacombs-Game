#pragma once
#include <SFML/Graphics.hpp>
#include "AbstractEntity.h"

class HealthBar : public AbstractEntity
{
public:
    HealthBar(sf::Vector2f pos, float width, float height, int maxHealth) : HealthBar(pos.x, pos.y, width, height, maxHealth) {

    }

    HealthBar(float x, float y, float width, float height, int maxHealth)
        : m_width(width), m_height(height), m_maxHealth(maxHealth), m_currentHealth(maxHealth) {
        // Background bar (gray)
        m_background.setSize(sf::Vector2f(m_width, m_height));
        //m_background.setFillColor(sf::Color(50, 50, 50));
        m_background.setFillColor(sf::Color(255, 0, 0));
        m_background.setPosition(x, y);

        // Foreground bar (green)
        m_foreground.setSize(sf::Vector2f(m_width, m_height));
        m_foreground.setFillColor(sf::Color(0, 255, 0)); // Green for health
        m_foreground.setPosition(x, y);
    }

    void SetHealth(int health) {
        m_currentHealth = health;
    }

    void SetPosition(sf::Vector2f pos) {
        m_background.setPosition(pos);
        m_foreground.setPosition(pos);
    }

    void SetOrigin(sf::Vector2f pos) {
        m_background.setOrigin( pos );
        m_foreground.setOrigin(pos);
    }

    void Update() {
        float healthPercentage = m_currentHealth / (float) m_maxHealth;
        m_foreground.setSize(sf::Vector2f(m_width * healthPercentage, m_height));
    }

    void Draw(sf::RenderWindow& window) {
        window.draw(m_background);   // Draw the background bar
        window.draw(m_foreground);   // Draw the foreground bar (health)
    }

private:

    float m_width;
    float m_height;
    int m_maxHealth;
    int m_currentHealth;

    sf::RectangleShape m_background;
    sf::RectangleShape m_foreground;
};
