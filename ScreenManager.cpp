#include "ScreenManager.h"
#include "TitleScreen.h"
#include "GameScreen.h"
#include <iostream>

ScreenManager::ScreenManager(sf::Font& font, sf::RenderWindow& window) {
	this->font = &font;
	this->screenSize = sf::Vector2f(window.getSize());
	this->loadScreen(ScreenEnum::titleScreen);
}

ScreenManager::~ScreenManager() {
	if (this->currentScreen != nullptr) {
		delete this->currentScreen;
		this->currentScreen = nullptr;
	}
}

void ScreenManager::loadScreen(int screenNum) {
	Screen* newScreen = nullptr;
	switch (screenNum) {
	case ScreenEnum::titleScreen:
		newScreen = new TitleScreen(screenSize);
		newScreen->nextScreen = ScreenEnum::gameScreen;
		break;
	case ScreenEnum::gameScreen:
		newScreen = new GameScreen(screenSize);
		newScreen->nextScreen = ScreenEnum::titleScreen;
		break;
	default:
		std::cout << "Unknown screen requested" << std::endl;
		break;
	}

	// if newScreen was assigned, switch to it.
	if (newScreen != nullptr) {
		Screen* oldScreen = this->currentScreen;
		this->currentScreen = newScreen;
		if (oldScreen != nullptr) {
			this->currentScreen->gameMode = oldScreen->gameMode;
		}

		delete oldScreen;
		oldScreen = nullptr;
	}
	else {
		std::cout << "Failed to load screen" << std::endl;
	}
}

void ScreenManager::update(float dt, InputManager& inputs) {
	if (this->currentScreen != nullptr) {
		if (this->currentScreen->goToNextScreen) {
			this->loadScreen(this->currentScreen->nextScreen);
		}
		this->currentScreen->update(dt, inputs);
	}
}

void ScreenManager::render(sf::RenderWindow& window, sf::Font& font) {
	if (this->currentScreen != nullptr) {
		this->currentScreen->render(window, font);
	}
	else {
		std::cout << "nothing to render..." << std::endl;
	}
}

void ScreenManager::inputHandler(sf::Event& event) {
	if (this->currentScreen != nullptr) {
		this->currentScreen->inputHandler(event);
	}
}