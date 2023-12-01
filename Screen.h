#pragma once
#include <SFML/Graphics.hpp>
#include "InputManager.h"

enum screenMode {
	singlePlayer,
	twoPlayer
};

class Screen {
public:
	int currentState;
	bool goToNextScreen;
	int nextScreen;
	sf::Vector2f screenSize;
	screenMode gameMode;

	Screen();
	~Screen();
	virtual void update(float dt, InputManager& inputs) = 0;
	virtual void render(sf::RenderWindow& window, sf::Font& font) = 0;
	virtual void inputHandler(sf::Event& event) = 0;
};