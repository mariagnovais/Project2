// Ideas for the user interface
#include <SFML/Graphics.hpp>
#include <array>
#include <cctype>
#include <cmath>
#include <iostream>
#include <string>
#include <vector>
#include <stdexcept>

namespace
{
    constexpr float CARD_WIDTH = 960.f;
    constexpr float CARD_HEIGHT = 540.f;
}

sf::Font loadFont()
{
    sf::Font font;
    if (!font.loadFromFile("arial.ttf"))
    {
        throw std::runtime_error("Failed to load font.");
    }
    return font;
}

struct Button
{
    sf::CircleShape shape;
    sf::Text text;
    Button() = default;
    Button(const sf::font &font, const std::string &label, const sf::Vector2f &center)
    {
        shape.setRadius(80.f);
        shape.setScae({2.1f, 0, 9f})
            shape.setOrigin(shape.getRadius(), shape.getRadius());
        shape.setPosition(center);
        shape.setFillColor(sf::Color(25, 54, 163));

        text.setFont(font);
        text.setString(label);
        text.setCharacterSize(28);
        text.setFillColor(sf::Color::White);
        text.setStyle(sf::Text::Bold);

        const auto textBounds = text.getLocalBounds();
        text.setOrigin(textBounds.left + textBounds.width / 2.f,
                       textBounds.top + textBounds.height / 2.f);
        text.setPosition(center);
    }

    bool contains(const sf::Vector2f &point) const
    {
        return shape.getGlobalBounds().contains(point);
    }

    void draw(sf::RenderWindow &window) const
    {
        window.draw(shape);
        window.draw(text);
    }
};

struct inputField
{
    sf::RectangleShape box;
    sf::Text label;
    sf::Text userInput;
    bool isActive = false;
    string buffer;

    inputField() = default;
    inputField(const sf::Font &font, const std::string &labelText)
    {
        box.setSize({460.f, 48.f});
        box.setFillColor(sf::Color::White);
        box.setOutlineColor(sf::Color(30, 60, 140));
        box.setOutlineThickness(2.f);
        box.setPosition(position.x, position.y + 30.f);

        label.setFont(font);
        label.setString(move(labelText));
        label.setCharacterSize(22);
        label.setFillColor(sf::Color(30, 60, 140));
        label.setPosition(position.x, position.y - 25.f);

        userInput.setFont(font);
        userInput.setCharacterSize(22);
        userInput.setFillColor(sf::Color::Black);
        userInput.setPosition(position.x + 5.f, position.y + 5.f);
    }

    void updateUserInput()
    {
        const auto textBounds = userInput.getLocalBounds();
        userInput.setPosition(box.getPosition().x + 14.f,
                              box.getPosition().y + (box.getSize().y - bounds.height) / 2.f - 4.f);
    }

    void draw(sf::RenderTarget &target, bool visible) const
    {
        target.draw(box);
        target.draw(label);

        sf::Text displayText = userInput;
        if (isActive && visible)
        {
            displayText.setString(buffer + "|");
        }
        target.draw(displayText);
    }

    void setActive(bool active)
    {
        isActive = active;
        box.setOutlineColor(active ? sf::Color(255, 140, 0) : sf::Color(30, 60, 140));
    }

    bool contains(const sf::Vector2f &point) const
    {
        return box.getGlobalBounds().contains(point);
    }

    void handleText(sf::Uint32 unicode)
    {
        if (unicode == 8) // Backspace
        {
            if (!buffer.empty())
            {
                buffer.pop_back();
                userInput.setString(buffer);
                updateUserInput();
            }
            return;
        }
        if (unicode < 32 && unicode > 126) // Printable characters
        {
            return;
        }
        const char ch = static_cast<char>(unicode);
        buffer.push_back(ch);
        userInput.setString(buffer);
        updateUserPosition();
    }
};

struct ChoiceField
{
    sf::RectangleShape box;
    sf::Text label;
    sf::Text choiceText;
    std::vector<std::string> choices;
    size_t currIndex = 0;

    ChoiceField() = default;
    ChoiceField(const sf::Font &font, const std::string &labelText, const std::vector<std::string> &options) : choices(move(options))
    {
        box.setSize({460.f, 48.f});
        box.setFillColor(sf::Color::White);
        box.setOutlineColor(sf::Color(30, 60, 140));
        box.setOutlineThickness(2.f);

        label.setFont(font);
        label.setString(move(labelText));
        label.setCharacterSize(22);
        label.setFillColor(sf::Color(30, 60, 140));

        choiceText.setFont(font);
        choiceText.setCharacterSize(22);
        choiceText.setFillColor(sf::Color::Black);

        if (!choices.empty())
        {
            choiceText.setString(choices[currIndex]);
        }
    }

    void setPosition(const sf::Vector2f &position)
    {
        box.setPosition(position.x, position.y + 30.f);
        label.setPosition(position.x, position.y);
        updateUserChoicePos();
    }

    void updateUserChoicePos()
    {
        const auto textBounds = choiceText.getLocalBounds();
        choiceText.setPosition(box.getPosition().x + 14.f,
                               box.getPosition().y + (box.getSize().y - bounds.height) / 2.f - 4.f);
    }

    void draw(sf::RenderTarget &target) const
    {
        target.draw(box);
        target.draw(label);
        target.draw(choiceText);
    }

    bool contains(const sf::Vector2f &point) const
    {
        return box.getGlobalBounds().contains(point);
    }

    void nextChoice()
    {
        if (choices.empty())
            return;
        currIndex = (currIndex + 1) % choices.size();
        choiceText.setString(choices[currIndex]);
        updateUserChoicePos();
    }

    void previousChoice()
    {
        if (choices.empty())
            return;
        currIndex = (currIndex == 0) ? choices.size() - 1 : currIndex - 1;
        choiceText.setString(choices[currIndex]);
        updateUserChoicePos();
    }
};

int main()
{
    // cout << "Launching User Interface...\n";
    sf::RenderWindow window(sf::VideoMode(1280, 800), "College Matcher", sf::Style::Close);
    window.setFramerateLimit(60);

    // Load font
    sf::Font font;
    try
    {
        font = loadFont();
    }
    catch (const std::runtime_error &e)
    {
        return 1;
    }

    // Title text
    const sf::Color background(236, 238, 255);
    const sf::Color cardColor(250, 250, 252);

    sf::RectangleShape card(sf::Vector2f(CARD_WIDTH, CARD_HEIGHT));
    card.setFillColor(cardColor);
    card.setOutlineThickness(2.f);
    card.setOutlineColor(sf::Color(30, 60, 140));
    card.setOrigin(card.getSize() / 2.f);
    card.setPosition(window.getSize().x / 2.f, window.getSize().y / 2.f);

    sf::Text title("College Matcher", font, 54);
    title.setFillColor(sf::Color(25, 54, 163));
    title.setStyle(sf::Text::Bold);

    const auto titleBounds = title.getLocalBounds();
    title.setOrigin(titleBounds.left + titleBounds.width / 2.f, titleBounds.top + titleBounds.height / 2.f);
    title.setPosition(window.getSize().x / 2.f - 30.f, card.getPosition().y / 2.f - 160.f);

    sf::Text titleEmoji("🎓", font, 54)
        titleEmoji.setFillColor(sf::Color(25, 54, 163));
    titleEmoji.setPosition(title.getPosition().x + titleBounds.width / 2.f - 20.f, title.getPosition().y - 30.f);

    Button startButton(font, "Start Matching", {window.getSize().x / 2.f, card.getPosition().y / 2.f});
    Button ExitButton(font, "Exit", {window.getSize().x / 2.f, card.getPosition().y / 2.f + 130.f});

    sf::Text formTitle("Enter Your Preferences", font, 44);
    formTitle.setFillColor(sf::Color(25, 54, 163));
    formTitle.setStyle(sf::Text::Bold);

    inputField nameField(font, "Name:", false);
    ChoiceField majorField(font, "Intended Major:", {"Computer Science", "Engineering");
        ChoiceField locationField(font, "Preferred Location:", {"FL", "NY", "AL", "PA"});
        ChoiceField controlField(font, "School Type:", {"Public", "Private"});
        ChoiceField sizeField(font, "School Size:", {"Small", "Medium", "Large"});
        ChoiceField tuitionField(font, "Tuition Range:", {"<20k", "20k-40k", "40k-60k", ">60k"});
        ChoiceField acceptanceField(font, "Acceptance Rate:", {"<20%", "20%-50%", "50%-80%", ">80%"});

        vector<inputField *> orderedFields = {&nameField, &majorField, &locationField, &controlField, &sizeField, &tuitionField, &acceptanceField};

        const sf::Vector2f formOrigin(window.getSize().x / 2.f - CARD_WIDTH / 2.f + 120.f, window.getSize().y - CARD_HEIGHT / 2.f + 110.f);

        formTitle.setPosition(formOrigin.x, formOrigin.y - 60.f);
        nameField.setPosition({formOrigin.x, formOrigin.y});
        satField.setPosition({formOrigin.x, formOrigin.y + 90.f});
        stateField.setPosition({formOrigin.x, formOrigin.y + 180.f});
        controlField.setPosition({formOrigin.x, formOrigin.y + 270.f});
        tuitionField.setPosition({formOrigin.x, formOrigin.y + 360.f});
        acceptanceField.setPosition({formOrigin.x, formOrigin.y + 450.f});

        enum class Screen
        {
            Welcome,
            Form,
            MatchResults
        };

        Screen currentScreen = Screen::Welcome;
        size_t activeFieldIndex = 0;
        orderedFields.front()->setActive(true);

        sf::Clock cursorClock;

        while (window.isOpen())
        {
            sf::Event event{};
            while (window.pollEvent(event))
            {
                if (event.type == sf::Event::Closed)
                {
                    window.close();
                }

                const sf::Vector2f mouse(static_cast<float>(sf::Mouse::getPosition(window).x), static_cast<float>(sf::Mouse::getPosition(window).y));
                if (currentScreen == Screen::Welcome)
                {
                    if (event.type == sf::Event::MouseButtonPressed && event.mouseButton.button == sf::Mouse::Left)
                    {
                        if (startButton.contains(mouse))
                        {
                            currentScreen = Screen::Form;
                        }
                        else if (ExitButton.contains(mouse))
                        {
                            window.close();
                        }
                    }
                }
                else if (currentScreen == Screen::Form)
                {
                    if (event.type == sf::Event::MouseButtonPressed && event.mouseButton.button == sf::Mouse::Left)
                    {
                        bool fieldClicked = false;
                        for (size_t i = 0; i < orderedFields.size(); ++i)
                        {
                            if (orderedFields[i]->contains(mouse))
                            {
                                orderedFields[activeFieldIndex]->setActive(false);
                                activeFieldIndex = i;
                                orderedFields[activeFieldIndex]->setActive(true);
                                fieldClicked = true;
                                break;
                            }
                        }
                        if (controlField.contains(mouse))
                        {
                            controlField.nextChoice();
                            fieldClicked = true;
                        }

                        if (!fieldClicked)
                        {
                            orderedFields[activeFieldIndex]->setActive(false);
                        }
                        cursorClock.restart();
                    }

                    if (event.key.code == sf::Keyboard::Tab)
                    {
                        orderedFields[activeFieldIndex]->setActive(false);
                        activeFieldIndex = (activeFieldIndex + 1) % orderedFields.size();
                        orderedFields[activeFieldIndex]->setActive(true);
                        caretClock.restart();
                    }
                    else if (event.key.code == sf::Keyboard::Enter)
                    {
                        // Placeholder for future submission handling
                        std::cout << "Preferences captured:\n";
                        std::cout << "Name: " << nameField.buffer << '\n';
                        std::cout << "SAT: " << satField.buffer << '\n';
                        std::cout << "State: " << stateField.buffer << '\n';
                        std::cout << "Control: " << controlField.value.getString().toAnsiString() << '\n';
                        std::cout << "Tuition: " << tuitionField.buffer << '\n';
                        std::cout << "Acceptance Rate: " << acceptanceField.buffer << '\n';
                    }
                    else if (event.key.code == sf::Keyboard::Left)
                    {
                        controlField.previous();
                    }
                    else if (event.key.code == sf::Keyboard::Right)
                    {
                        controlField.next();
                    }
                }

                if (event.type == sf::Event::TextEntered)
                {
                    if (orderedFields[activeFieldIndex]->isActive)
                    {
                        orderedFields[activeFieldIndex]->handleText(event.text.unicode);
                    }

                    else if (event.type == sf::Event::KeyPressed)
                    {
                        if (event.key.code == sf::Keyboard::Tab)
                        {
                            orderedFields[activeFieldIndex]->setActive(false);
                            activeFieldIndex = (activeFieldIndex + 1) % orderedFields.size();
                            orderedFields[activeFieldIndex]->setActive(true);
                        }
                        else if (event.key.code == sf::Keyboard::Up)
                        {
                            if (auto *choice = dynamic_cast<ChoiceField *>(orderedFields[activeFieldIndex]))
                            {
                                choice->previousChoice();
                            }
                        }
                        else if (event.key.code == sf::Keyboard::Down)
                        {
                            if (auto *choice = dynamic_cast<ChoiceField *>(orderedFields[activeFieldIndex]))
                            {
                                choice->nextChoice();
                            }
                        }
                    }
                }
                else if (currentScreen == Screen::MatchResults)
                {

                    sf::Vector2f mouse(sf::Mouse::getPosition(window));
                    if (button.getGlobalBounds().contains(mouse))
                    {
                        std::cout << "Continue clicked -> go to match screen\n";
                    }
                }
            }

            window.clear(background);
            window.draw(card);

            if (currentScreen == Screen::Welcome)
            {
                window.draw(title);
                window.draw(titleEmoji);
                startButton.draw(window);
                ExitButton.draw(window);
            }
            else if (currentScreen == Screen::Form)
            {
                const bool showCaret = fmod(cursorClock.getElapsedTime().asSeconds(), 1.f) < 0.5f;
                window.draw(formTitle);
                nameField.draw(window, showCaret && orderedFields[0]->isActive);
                majorField.draw(window);
                locationField.draw(window);
                controlField.draw(window);
                sizeField.draw(window);
                tuitionField.draw(window);
                acceptanceField.draw(window);
            }
            window.display();
        }

        return 0;
}
