#include "InputManager.h"
#include <iostream>

Input::Input(sf::Keyboard::Key inputNum) {
	this->inputNum = inputNum;
	this->justPressed = false;
	this->previousInput = false;
	this->currentInput = false;
	this->justReleased = false;
}

void Input::update() {
	if (sf::Keyboard::isKeyPressed(this->inputNum)) {
		if (!previousInput) {
			justPressed = true;
		}
		currentInput = true;
		previousInput = true;
	}
	else {
		if (previousInput) {
			justReleased = true;
		}
		currentInput = false;
		previousInput = false;
	}
}

void Input::pressAndReleaseReset() {
	if (sf::Keyboard::isKeyPressed(this->inputNum)) {
		justPressed = false;
		justReleased = false;
	}
}


InputManager::InputManager(){
	this->enter = new Input(sf::Keyboard::Enter);
	this->up = new Input(sf::Keyboard::Up);
	this->down = new Input(sf::Keyboard::Down);
	this->w = new Input(sf::Keyboard::W);
	this->s = new Input(sf::Keyboard::S);
	this->p = new Input(sf::Keyboard::P);
	this->one = new Input(sf::Keyboard::Num1);
	this->two = new Input(sf::Keyboard::Num2);
}

InputManager::~InputManager() {
	delete this->enter;
	delete this->up;
	delete this->down;
	delete this->w;
	delete this->s;
	delete this->p;
	delete this->one;
	delete this->two;
}

void InputManager::update() {
	this->enter->update();
	this->up->update();
	this->down->update();
	this->w->update();
	this->s->update();
	this->p->update();
	this->one->update();
	this->two->update();
}

void InputManager::pressAndReleaseReset() {
	this->enter->pressAndReleaseReset();
	this->up->pressAndReleaseReset();
	this->down->pressAndReleaseReset();
	this->w->pressAndReleaseReset();
	this->s->pressAndReleaseReset();
	this->p->pressAndReleaseReset();
	this->one->pressAndReleaseReset();
	this->two->pressAndReleaseReset();
}