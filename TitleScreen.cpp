#include "TitleScreen.h"
#include <iostream>

TitleScreen::TitleScreen(sf::Vector2f screenSize) {
	this->currentState = titleStatesEnum::title;
	this->goToNextScreen = false;
	this->screenSize = screenSize;
}

TitleScreen::~TitleScreen() {

}

void TitleScreen::update(float dt, InputManager& inputs) {
	switch (this->currentState) {
	case titleStatesEnum::title:
		if (inputs.enter->justPressed) {
			this->currentState = titleStatesEnum::modeSelect;
			std::cout << "enter" << std::endl;
		}
		break;
	case titleStatesEnum::modeSelect:
		if (inputs.one->justPressed) {
			this->goToNextScreen = true;
			gameMode = screenMode::singlePlayer;
		}else if (inputs.two->justPressed) {
			this->goToNextScreen = true;
			gameMode = screenMode::twoPlayer;
		}
		break;
	default:
		std::cout << "unknown state" << std::endl;
		break;
	}
}

void TitleScreen::render(sf::RenderWindow& window, sf::Font& font) {
	sf::Text title("PONG", font, 300);
	title.setFillColor(sf::Color::White);
	title.setOrigin(title.getLocalBounds().width / 2, title.getLocalBounds().height / 2);
	title.setPosition(screenSize.x / 2, 50.0f);
	window.draw(title);

	sf::Text subtitle("Subtitle", font, 24);
	subtitle.setFillColor(sf::Color::White);
	subtitle.setPosition(screenSize.x / 2, 350.0f);

	switch (this->currentState) {
	case titleStatesEnum::title:
		subtitle.setString("Press Enter to start\n\n- by Charles Park");
		subtitle.setOrigin(subtitle.getLocalBounds().width / 2, subtitle.getLocalBounds().height / 2);
		window.draw(subtitle);
		break;
	case titleStatesEnum::modeSelect:
		subtitle.setString("Mode Select: \nPress 1 to play against AI\nPress 2 for 2 Players");
		subtitle.setOrigin(subtitle.getLocalBounds().width / 2, subtitle.getLocalBounds().height / 2);
		window.draw(subtitle);
		break;
	default:
		break;
	}
}

void TitleScreen::inputHandler(sf::Event& event) {
}