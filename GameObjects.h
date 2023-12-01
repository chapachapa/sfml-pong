#pragma once
#include <SFML/System.hpp>
#include <SFML/Graphics.hpp>

enum ObjectShape {
	Circle,
	Rectangle
};

enum rectOrientation {
	horizontal,
	vertical
};

enum rectType {
	wall,
	goal1,
	goal2,
	paddle1,
	paddle2
};

class CollidableObject {
public:
	ObjectShape shapeId;
	sf::Vector2f position;
	CollidableObject();
	virtual void update(float dt) = 0;
	virtual void render(sf::RenderWindow& window) = 0;
};

class Ball : public CollidableObject{
public:
	CollidableObject* lastCollided;
	sf::CircleShape circle;
	sf::Vector2f velocity;
	sf::Color color;
	float currentSpeed;
	float startingSpeed;
	float radius;
	float speedMultiplier;
	bool isMoving;
	bool showBall;
	Ball(float radius);
	void update(float dt);
	void render(sf::RenderWindow& window);
	void startNewBall();
private:
	int rollRandomPositiveInteger(int maxValue_exclusive);
};

class CollidableRectangle : public CollidableObject {
public:
	rectType rectId;
	sf::RectangleShape rectangle;
	sf::Color color;
	sf::Vector2f dimension;
	rectOrientation orientation;
	CollidableRectangle(sf::Vector2f position, sf::Vector2f dimension, 
		rectOrientation orientation, rectType rectId);
	void update(float dt);
	void render(sf::RenderWindow& window);
};

//class Paddle : public CollidableRectangle {
//public:
//	Paddle(sf::Vector2f position, sf::Vector2f dimension,
//		rectOrientation orientation, rectType rectId);
//	void update(float dt);
//	void render(sf::RenderWindow& window);
//};