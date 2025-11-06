#pragma once
#include <iostream>
#include <SFML/Graphics.hpp>
#include <string>

class Button
{
    sf::Sprite sprite;
    sf::Vector2f position;
    std::function<void(void)> buttonClicked;
    public:
    Button(sf::Vector2f _position, std::function<void(void)> _onClick);
    sf::Vector2f getPosition(); // returns position of button
    sf::Sprite *getSprite(); // returns sprite of button
    void setSprite(sf::Sprite _sprite); // sets sprite
    void onClick() { buttonClicked(); }; // invokes callback method when button clicked
}

class window
{
public:
    sf::RenderWindow window;
    // user clicks on start
    Button *starting;
    // user clicks on exit
    Button *exit;
};