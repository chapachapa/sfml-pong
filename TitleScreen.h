#pragma once

#include "Screen.h"

enum titleStatesEnum {
	title,
	modeSelect
};

class TitleScreen : public Screen {
public:
	TitleScreen(sf::Vector2f screenSize);
	~TitleScreen();
	void update(float dt, InputManager& inputs);
	void render(sf::RenderWindow& window, sf::Font& font);
	void inputHandler(sf::Event& event);
};