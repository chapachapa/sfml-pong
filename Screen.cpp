#include "Screen.h"
#include <iostream>

Screen:: Screen() {
	std::cout << "Screen is instantiated" << std::endl;
}

Screen :: ~Screen() {
	std::cout << "destructor called" << std::endl;
}

//void Screen::update(int dt) {
//	std::cout << "Screen updated" << std::endl;
//}
//
//void Screen::render() {
//	std::cout << "screen rendered" << std::endl;
//}
//
//void Screen::inputHandler() {
//	std::cout << "keypress detected" << std::endl;
//}