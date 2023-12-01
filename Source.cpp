#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>
#include <SFML/Audio.hpp>
#include <SFML/Network.hpp>
#include <SFML/System.hpp>
#include <SFML/OpenGL.hpp>
#include <SFML/Main.hpp>
#include <time.h>
#include <iostream>

#include "ScreenManager.h"
#include "InputManager.h"

int main()
{
    sf::RenderWindow window(sf::VideoMode(1600, 900), "Pong");

    static sf::Font font;
    if (!font.loadFromFile("visitor1.ttf")) {
        std::cout << "unable to load font. Sometimes this happens please try again" << std::endl;
    }
    sf::Vector2f test;
    ScreenManager screen(font, window);
    InputManager inputs;
    sf::Clock deltaSrc;
    bool isPaused = false;

    srand(time(NULL));
    
    while (window.isOpen())
    {
        sf::Time dt_time = deltaSrc.restart();
        float dt = dt_time.asSeconds();

        sf::Event event;
        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed) {
                window.close();
            }
            inputs.update();
        }


        window.clear();
        if (!isPaused) {
            screen.update(dt, inputs);
        }
        else {
            sf::RectangleShape pauseScreen(sf::Vector2f(window.getSize()));
            pauseScreen.setPosition(0, 0);
            pauseScreen.setFillColor(sf::Color(255, 255, 255, 50));
            window.draw(pauseScreen);
            sf::Text pauseText("- Paused -", font, 30);
            pauseText.setFillColor(sf::Color::Yellow);
            pauseText.setPosition(window.getSize().x / 2, window.getSize().y / 2);
            pauseText.setOrigin(pauseText.getLocalBounds().width / 2, pauseText.getLocalBounds().height / 2);
            
            window.draw(pauseText);
        }

        screen.render(window, font);
        window.display();

        if (inputs.p->justPressed) {
            isPaused = !isPaused;
        }

        inputs.pressAndReleaseReset();

    }
    return 0;
}