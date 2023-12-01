#pragma once
#include "Screen.h"
#include "InputManager.h"
#include <SFML/Main.hpp>

enum ScreenEnum {
	titleScreen,
	gameScreen
};

class ScreenManager {
public:
	Screen* currentScreen;
	sf::Font* font;
	sf::Vector2f screenSize;
	ScreenManager(sf::Font& font, sf::RenderWindow& window);
	~ScreenManager();
	void loadScreen(int screenNum);
	void update(float dt, InputManager& inputs);
	void render(sf::RenderWindow& window, sf::Font& font);
	void inputHandler(sf::Event& event);
private:
};