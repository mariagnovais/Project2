// Ideas for the user interface
#include <SFML/Graphics.hpp>
#include <iostream>

int main()
{
    // cout << "Launching User Interface...\n";
    sf::RenderWindow window(sf::VideoMode(800, 600), "College Matcher", sf::Style::Close);
    window.setFramerateLimit(60);

    // Colors
    sf::Color bgColor(245, 245, 245);
    sf::Color borderColor(255, 140, 0);
    sf::Color blue(46, 93, 195);
    sf::Color blueDark(30, 60, 140);

    // Card (the white centered box)
    sf::RectangleShape card(sf::Vector2f(420, 350));
    card.setFillColor(sf::Color::White);
    card.setOutlineThickness(3);
    card.setOutlineColor(borderColor);
    card.setOrigin(card.getSize() / 2.f);
    card.setPosition(window.getSize().x / 2.f, window.getSize().y / 2.f);

    // Load font
    sf::Font font;
    if (!font.loadFromFile("arial.ttf"))
    {
        std::cout << "Failed to load font.\n";
        return 1;
    }

    // Title text
    sf::Text title("🎓 College", font, 32);
    title.setFillColor(blue);
    title.setStyle(sf::Text::Bold);
    title.setPosition(card.getPosition().x - title.getGlobalBounds().width / 2.f,
                      card.getPosition().y - 120);

    // Group text
    sf::Text group("Group 47", font, 22);
    group.setFillColor(blue);
    group.setStyle(sf::Text::Bold);
    group.setPosition(card.getPosition().x - group.getGlobalBounds().width / 2.f,
                      card.getPosition().y - 30);

    // Members text
    sf::Text members("Caroline • Maria • Theron", font, 20);
    members.setFillColor(blueDark);
    members.setPosition(card.getPosition().x - members.getGlobalBounds().width / 2.f,
                        card.getPosition().y + 5);

    // Continue button
    sf::RectangleShape button(sf::Vector2f(160, 45));
    button.setFillColor(sf::Color(240, 240, 240));
    button.setOutlineThickness(0);
    button.setOrigin(button.getSize() / 2.f);
    button.setPosition(card.getPosition().x, card.getPosition().y + 100);

    sf::Text buttonText("[CONTINUE]", font, 18);
    buttonText.setFillColor(blueDark);
    buttonText.setStyle(sf::Text::Bold);
    buttonText.setPosition(
        button.getPosition().x - buttonText.getGlobalBounds().width / 2.f,
        button.getPosition().y - buttonText.getGlobalBounds().height / 2.f - 5);

    // Main loop
    while (window.isOpen())
    {
        sf::Event event;
        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
                window.close();

            // Click detection
            if (event.type == sf::Event::MouseButtonPressed &&
                event.mouseButton.button == sf::Mouse::Left)
            {

                sf::Vector2f mouse(sf::Mouse::getPosition(window));
                if (button.getGlobalBounds().contains(mouse))
                {
                    std::cout << "Continue clicked -> go to match screen\n";
                }
            }
        }

        window.clear(bgColor);
        window.draw(card);
        window.draw(title);
        window.draw(group);
        window.draw(members);
        window.draw(button);
        window.draw(buttonText);
        window.display();
    }

    return 0;
}
