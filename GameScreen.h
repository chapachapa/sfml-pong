#pragma once

#include "Screen.h"
#include "GameObjects.h"
#include <vector>

enum gameStates {
	idle,
	play,
	score,
	win
};

enum aiStates {
	idleAi,
	calculating,
	waiting,
	tracking
};

class GameScreen : public Screen {
public:
	gameStates currentState;
	aiStates aiState;
	float aiTargetHeight;
	float aiIdleTarget;
	Ball* ball;
	CollidableRectangle* paddle1;
	CollidableRectangle* paddle2;
	float timeToAIReaction;
	float timeToNextAIIdleTarget;
	float startCount;
	float blinkTimeCount;
	int paddleHitCount;
	int player1Score;
	int player2Score;
	bool player1Scored;
	bool player2Scored;
	std::vector<CollidableRectangle*> objects;
	GameScreen(sf::Vector2f screenSize);
	void resetRound();
	void update(float dt, InputManager& inputs);
	void render(sf::RenderWindow& window, sf::Font& font);
	void inputHandler(sf::Event& event);
	sf::Vector2f clamp(sf::Vector2f value, sf::Vector2f min, sf::Vector2f max);
	bool checkBallCollision(Ball& ball, CollidableRectangle& rectObj);
private:
	sf::Vector2f getPrediction();
	float getIdleTargetY();
	void renderWinScreen(sf::RenderWindow& window, sf::Font& font);
};