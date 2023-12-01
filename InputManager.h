#pragma once
#include <SFML/Main.hpp>
#include <SFML/Window.hpp>

class Input {
public:
	bool justPressed;
	bool currentInput;
	bool previousInput;
	bool justReleased;
	sf::Keyboard::Key inputNum; // Enum from sf::Input
	Input(sf::Keyboard::Key inputNum);
	void update();
	void pressAndReleaseReset();
};

class InputManager {
public:
	Input* enter;
	Input* up;
	Input* down;
	Input* w;
	Input* s;
	Input* p;
	Input* one;
	Input* two;
	InputManager();
	~InputManager();
	void update();
	void pressAndReleaseReset();
};