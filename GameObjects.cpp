#include "GameObjects.h"
#include <iostream>

const float PI = 3.141592654;

CollidableObject::CollidableObject() {
	this->position = sf::Vector2f(0, 0);
}

Ball::Ball(float radius) {
	this->shapeId = ObjectShape::Circle;
	this->radius = radius;
	this->velocity = sf::Vector2f(0, 0);
	this->position = sf::Vector2f(0, 0);
	this->circle = sf::CircleShape(radius);
	this->color = sf::Color::White;
	this->startingSpeed = 400.0f;
	this->currentSpeed = 0.0f;
	this->speedMultiplier = 1.1f;
	this->isMoving = true;
	this->showBall = true;

	startNewBall();
}

void Ball::update(float dt) {
	if (isMoving && currentSpeed <= 0.0f) {
		currentSpeed = startingSpeed;
	}
	position += velocity * currentSpeed * dt;
}

void Ball::render(sf::RenderWindow& window) {
	if (showBall) {
		circle.setOrigin(radius, radius);
		circle.setFillColor(color);
		circle.setPosition(position);
		window.draw(circle);
	}
}

void Ball::startNewBall() {
	float randomAngleInDegrees = rollRandomPositiveInteger(90) + 45.0f + (rollRandomPositiveInteger(2) == 0 ? 0 : 180.0f);
	//std::cout << "selected angle:::" << std::to_string(randomAngleInDegrees) << std::endl;
	float randomAngleInRadians = (randomAngleInDegrees * PI / 360.0f);
	auto randomDirection = sf::Vector2f(
		cos(randomAngleInRadians),
		sin(randomAngleInRadians)
	);
	this->currentSpeed = this->startingSpeed;
	this->velocity = randomDirection;
}

int Ball::rollRandomPositiveInteger(int maxValue_exclusive) {
	return abs(rand()) % maxValue_exclusive;
}

CollidableRectangle::CollidableRectangle(sf::Vector2f position, sf::Vector2f dimension, rectOrientation orientation, rectType rectId) {
	this->shapeId = ObjectShape::Rectangle;
	this->dimension = dimension;
	this->color = sf::Color::White;
	this->position = position;
	this->orientation = orientation;
	this->rectId = rectId;
}

void CollidableRectangle::update(float dt) {
	return;
}

void CollidableRectangle::render(sf::RenderWindow& window) {
	rectangle.setFillColor(color);
	rectangle.setPosition(position);
	rectangle.setSize(dimension);
	window.draw(rectangle);
}