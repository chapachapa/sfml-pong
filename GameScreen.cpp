#include "GameScreen.h"
#include <iostream>

const float WALL_DEPTH = 20;
const float CENTER_LINE_WIDTH = 10;
const float PADDLE_LENGTH = 150;
const float PADDLE_WIDTH = 20;
const float PADDLE_OFFSET_FROM_WALL = 150;
const float PADDLE_SPEED = 850;
const float AI_PADDLE_SPEED = 650;
const float AI_IDLE_SPEED = 50;
const float BALL_SPEED_MULTIPLIER = 1.1;

const float TIME_BEFORE_STARTING_GAME = 2.0f;
const float BLINK_TIME = 1.0f;
const float BLINK_INTERVAL = 0.20f;

const int POINTS_TO_WIN = 10;

GameScreen::GameScreen(sf::Vector2f screenSize) {
	this->currentState = gameStates::idle;
	this->player1Score = 0;
	this->player2Score = 0;
	this->startCount = 0;
	this->blinkTimeCount = 0;
	this->screenSize = screenSize;
	this->player1Scored = false;
	this->player2Scored = false;
	this->aiTargetHeight = 0;
	this->aiState = aiStates::idleAi;
	this->paddleHitCount = 0;
	this->aiIdleTarget = getIdleTargetY();

	ball = new Ball(15);
	ball->position = sf::Vector2f(screenSize.x/2, screenSize.y/2);
	ball->color = sf::Color::Red;
	ball->speedMultiplier = BALL_SPEED_MULTIPLIER;

	paddle1 = new CollidableRectangle(sf::Vector2f(PADDLE_OFFSET_FROM_WALL, (screenSize.y / 2) - (PADDLE_LENGTH / 2)),
		sf::Vector2f(PADDLE_WIDTH, PADDLE_LENGTH), rectOrientation::vertical, rectType::paddle1);
	paddle2 = new CollidableRectangle(sf::Vector2f(screenSize.x - PADDLE_OFFSET_FROM_WALL - PADDLE_WIDTH, (screenSize.y / 2) - (PADDLE_LENGTH / 2)),
		sf::Vector2f(PADDLE_WIDTH, PADDLE_LENGTH), rectOrientation::vertical, rectType::paddle2);

	std::cout << std::to_string(screenSize.x) << ", " << std::to_string(screenSize.y) << std::endl;
	objects.push_back(new CollidableRectangle(sf::Vector2f(0, 0), sf::Vector2f(screenSize.x, WALL_DEPTH), 
		rectOrientation::horizontal, rectType::wall));
	objects.push_back(new CollidableRectangle(sf::Vector2f(0, screenSize.y - WALL_DEPTH), sf::Vector2f(screenSize.x, WALL_DEPTH), 
		rectOrientation::horizontal, rectType::wall));
	objects.push_back(new CollidableRectangle(sf::Vector2f(0, 0), sf::Vector2f(WALL_DEPTH, screenSize.y), 
		rectOrientation::vertical, rectType::goal1));
	objects.push_back(new CollidableRectangle(sf::Vector2f(screenSize.x - WALL_DEPTH, 0), sf::Vector2f(WALL_DEPTH, screenSize.y), 
		rectOrientation::vertical, rectType::goal2));
	objects.push_back(paddle1);
	objects.push_back(paddle2);
}

void GameScreen::resetRound() {
	ball->position = sf::Vector2f(screenSize.x / 2, screenSize.y / 2);
	ball->startNewBall();
	this->paddleHitCount = 0;
	this->player1Scored = false;
	this->player2Scored = false;
	ball->showBall = true;
}

void GameScreen::update(float dt, InputManager& inputs) {

	switch (currentState) {
	case gameStates::idle:
		startCount += dt;
		if (startCount > TIME_BEFORE_STARTING_GAME) {
			currentState = gameStates::play;
			startCount = 0;
		}
		break;
	case gameStates::play:
		break;
	case gameStates::score:
		blinkTimeCount += dt;
		ball->showBall = false;
		if (blinkTimeCount > BLINK_TIME ) {
			if (player1Score >= POINTS_TO_WIN || player2Score >= POINTS_TO_WIN) {
				currentState = gameStates::win;
			}
			else {
				currentState = gameStates::idle;
				blinkTimeCount = 0;
				resetRound();
			}
		}
		break;
	case gameStates::win:
		return;
		break;
	default:
		std::cout << "unknown gameState" << std::endl;
		break;
	}

	if (inputs.w->currentInput) {
		paddle1->position = clamp(paddle1->position + sf::Vector2f(0, -1 * PADDLE_SPEED * dt),
			sf::Vector2f(WALL_DEPTH, WALL_DEPTH), sf::Vector2f(screenSize.x - WALL_DEPTH, screenSize.y - WALL_DEPTH));
	}
	else if (inputs.s->currentInput) {
		paddle1->position = clamp(paddle1->position + sf::Vector2f(0, 1 * PADDLE_SPEED * dt), 
			sf::Vector2f(WALL_DEPTH, WALL_DEPTH), sf::Vector2f(screenSize.x-WALL_DEPTH, screenSize.y-WALL_DEPTH-PADDLE_LENGTH));
	}

	if (gameMode == screenMode::twoPlayer) {
		if (inputs.up->currentInput) {
			paddle2->position = clamp(paddle2->position + sf::Vector2f(0, -1 * PADDLE_SPEED * dt),
				sf::Vector2f(WALL_DEPTH, WALL_DEPTH), sf::Vector2f(screenSize.x - WALL_DEPTH, screenSize.y - WALL_DEPTH));
		}
		else if (inputs.down->currentInput) {
			paddle2->position = clamp(paddle2->position + sf::Vector2f(0, 1 * PADDLE_SPEED * dt),
				sf::Vector2f(WALL_DEPTH, WALL_DEPTH), sf::Vector2f(screenSize.x - WALL_DEPTH, screenSize.y - WALL_DEPTH - PADDLE_LENGTH));
		}
	}
	else {
		switch (aiState) {
		case aiStates::idleAi:
			if (ball->velocity.x > 0 && ball->position.x > screenSize.x / 2) {
				aiState = aiStates::calculating;
			}
			break;
		case aiStates::calculating:
			aiTargetHeight = getPrediction().y;
			timeToAIReaction = ((float)(rand()) / (float)(RAND_MAX/1.5 - (0.05*paddleHitCount)));
			aiState = aiStates::waiting;
			break;
		case aiStates::waiting:
			timeToAIReaction -= dt;
			if (timeToAIReaction < 0) {
				aiState = aiStates::tracking;
			}
			break;
		case aiStates::tracking:
			if (ball->velocity.x < 0) {
				aiIdleTarget = getIdleTargetY();
				timeToNextAIIdleTarget = ((float)(rand()) / (float)(RAND_MAX / 3));
				aiState = aiStates::idleAi;
			}
			break;
		default:
			std::cout << "unknown aiState" << std::endl;
			break;
		}
		// this is where AI controls it.
		const float MARGIN = 5;
		if (aiState == aiStates::tracking) {
			if (paddle2->position.y + (PADDLE_LENGTH/2) - MARGIN > aiTargetHeight) {
				paddle2->position = clamp(paddle2->position + sf::Vector2f(0, -1 * AI_PADDLE_SPEED * dt),
					sf::Vector2f(WALL_DEPTH, WALL_DEPTH), sf::Vector2f(screenSize.x - WALL_DEPTH, screenSize.y - WALL_DEPTH));
			}
			else if (paddle2->position.y + (PADDLE_LENGTH/2) + MARGIN < aiTargetHeight) {
				paddle2->position = clamp(paddle2->position + sf::Vector2f(0, 1 * AI_PADDLE_SPEED * dt),
					sf::Vector2f(WALL_DEPTH, WALL_DEPTH), sf::Vector2f(screenSize.x - WALL_DEPTH, screenSize.y - WALL_DEPTH - PADDLE_LENGTH));
			}
		}
		else {
			if (paddle2->position.y + (PADDLE_LENGTH / 2) - MARGIN > aiIdleTarget) {
				paddle2->position = clamp(paddle2->position + sf::Vector2f(0, -1 * AI_PADDLE_SPEED * dt),
					sf::Vector2f(WALL_DEPTH, WALL_DEPTH), sf::Vector2f(screenSize.x - WALL_DEPTH, screenSize.y - WALL_DEPTH));
			}
			else if (paddle2->position.y + (PADDLE_LENGTH / 2) + MARGIN < aiIdleTarget) {
				paddle2->position = clamp(paddle2->position + sf::Vector2f(0, 1 * AI_PADDLE_SPEED * dt),
					sf::Vector2f(WALL_DEPTH, WALL_DEPTH), sf::Vector2f(screenSize.x - WALL_DEPTH, screenSize.y - WALL_DEPTH - PADDLE_LENGTH));
			}
			else {
				// get new AI idle target
				timeToNextAIIdleTarget -= dt;
				if (timeToNextAIIdleTarget < 0) {
					aiIdleTarget = getIdleTargetY();
					timeToNextAIIdleTarget = ((float)(rand()) / (float)(RAND_MAX / 2));
				}
			}
		}
	}

	if (currentState == gameStates::play) {
		ball->update(dt);
		ball->position = clamp(ball->position, sf::Vector2f(0, 0), screenSize);
	}

	for (CollidableRectangle* obj : objects) {
		obj->update(dt);
		if (ball->lastCollided != obj && checkBallCollision(*ball, *obj)) {
			ball->lastCollided = obj;
			if (obj->orientation == rectOrientation::horizontal) {
				ball->velocity = sf::Vector2f(ball->velocity.x, -1.0 * ball->velocity.y);
			}
			else {
				ball->velocity = sf::Vector2f(-1.0 * ball->velocity.x, ball->velocity.y);
			}

			if (obj->rectId == rectType::paddle1 || obj->rectId == rectType::paddle2) {
				ball->currentSpeed *= ball->speedMultiplier;
				paddleHitCount++;
			}

			if (obj->rectId == rectType::goal1) {
				player2Score++;
				player2Scored = true;
				currentState = gameStates::score;
			}
			else if (obj->rectId == rectType::goal2) {
				player1Score++;
				player1Scored = true;
				currentState = gameStates::score;
			}
		}
		else {
		}
	}
}

const sf::Vector2f SCORE_POSITION_OFFSET(150, 100);
const int SCORE_FONT_SIZE = 120;

void GameScreen::render(sf::RenderWindow& window, sf::Font& font) {
	sf::Text player1ScoreText(std::to_string(player1Score), font, SCORE_FONT_SIZE);
	sf::Text player2ScoreText(std::to_string(player2Score), font, SCORE_FONT_SIZE);
	player1ScoreText.setOrigin(player1ScoreText.getLocalBounds().width / 2, player1ScoreText.getLocalBounds().height / 2);
	player1ScoreText.setPosition(sf::Vector2f(screenSize.x / 2, 0)
		+ sf::Vector2f(SCORE_POSITION_OFFSET.x * -1, SCORE_POSITION_OFFSET.y));
	player2ScoreText.setPosition(sf::Vector2f(screenSize.x / 2, 0) + SCORE_POSITION_OFFSET);
	player2ScoreText.setOrigin(player2ScoreText.getLocalBounds().width / 2, player2ScoreText.getLocalBounds().height / 2);

	if (currentState == gameStates::score) {
		if (player1Scored) {
			player1ScoreText.setFillColor(sf::Color(255, 255, 255, 
				(int)(blinkTimeCount / BLINK_INTERVAL) % 2 == 0 ? 0 : 255));
		}
		else if (player2Scored) {
			player2ScoreText.setFillColor(sf::Color(255, 255, 255,
				(int)(blinkTimeCount / BLINK_INTERVAL) % 2 == 0 ? 0 : 255));
		}
	}
	else {
		player1ScoreText.setFillColor(sf::Color::White);
		player2ScoreText.setFillColor(sf::Color::White);
	}

	window.draw(player1ScoreText);
	window.draw(player2ScoreText);


	sf::RectangleShape centerLine;
	centerLine.setPosition((screenSize.x / 2) - (CENTER_LINE_WIDTH / 2), 0);
	centerLine.setSize(sf::Vector2f(CENTER_LINE_WIDTH, screenSize.y));
	centerLine.setFillColor(sf::Color(255, 255, 255, 127));
	window.draw(centerLine);
	
	for (CollidableRectangle* obj : objects) {
		obj->render(window);
	}

	ball->render(window);

	if (currentState == gameStates::win) {
		renderWinScreen(window, font);
	}

	//visualization for prediction
	/*if (ball->velocity.x > 0) {
		sf::CircleShape circle(15.0f);
		circle.setPosition(getPrediction());
		circle.setFillColor(sf::Color::Blue);
		window.draw(circle);
	}

	sf::CircleShape idleTarget(15.0f);
	idleTarget.setPosition(screenSize.x-PADDLE_OFFSET_FROM_WALL, aiIdleTarget);
	idleTarget.setFillColor(sf::Color::Green);
	window.draw(idleTarget);*/
}

sf::Vector2f GameScreen::getPrediction() {
	float height = screenSize.y - (WALL_DEPTH * 2);
	float distanceToPaddle = (screenSize.x - PADDLE_OFFSET_FROM_WALL) - ball->position.x;
	float total = ball->velocity.y * (distanceToPaddle / ball->velocity.x);
	float projection = total - (height - (ball->position.y - WALL_DEPTH));
	int heightCount = (int)(projection / height);

	float moddedYPosition = abs((int)projection % (int)height);
	float sign = powf(-1, heightCount + (ball->position.y < 0 ? 1 : 0));

	if (sign > -1) {
		moddedYPosition = height - moddedYPosition;
	}

	return sf::Vector2f(screenSize.x - PADDLE_OFFSET_FROM_WALL, moddedYPosition + WALL_DEPTH);
}

float GameScreen::getIdleTargetY() {
	return WALL_DEPTH + PADDLE_LENGTH + rand() % (int)(screenSize.y - (PADDLE_LENGTH * 2) - (WALL_DEPTH * 2));
}

void GameScreen::inputHandler(sf::Event& event) {
}

sf::Vector2f GameScreen::clamp(sf::Vector2f value, sf::Vector2f min, sf::Vector2f max) {
	sf::Vector2f clampedVector(value);
	if (value.x < min.x) {
		clampedVector.x = min.x;
	}
	else if (value.x > max.x) {
		clampedVector.x = max.x;
	}

	if (value.y < min.y) {
		clampedVector.y = min.y;
	}
	else if (value.y > max.y) {
		clampedVector.y = max.y;
	}

	return clampedVector;
}

bool GameScreen::checkBallCollision(Ball& ball, CollidableRectangle& rectObj) {
	// https://www.jeffreythompson.org/collision-detection/circle-rect.php
	float testX = ball.position.x;
	float testY = ball.position.y;
	if (ball.position.x < rectObj.position.x) {
		testX = rectObj.position.x;
	}
	else if (ball.position.x > rectObj.position.x + rectObj.dimension.x) {
		testX = rectObj.position.x + rectObj.dimension.x;
	}
	// check top edge
	if (ball.position.y < rectObj.position.y) {
		testY = rectObj.position.y;
	}
	else if (ball.position.y > rectObj.position.y + rectObj.dimension.y) {
		testY = rectObj.position.y + rectObj.dimension.y;
	}

	float distX = ball.position.x - testX;
	float distY = ball.position.y - testY;
	float squaredDistance = (distX * distX) + (distY * distY);
	if (squaredDistance <= ball.radius * ball.radius) {
		return true;
	}
	return false;
}

void GameScreen::renderWinScreen(sf::RenderWindow& window, sf::Font& font) {
	std::string winString = "- GAME OVER -";
	if (player1Score >= 10) {
		winString = "- PLAYER 1 WINS! -";
	}
	else if (player2Score >= 10) {
		winString = "- PLAYER 2 WINS! -";
	}
	sf::RectangleShape winScreen(screenSize);
	winScreen.setPosition(0, 0);
	winScreen.setFillColor(sf::Color(255, 255, 255, 50));
	window.draw(winScreen);
	sf::Text winText(winString, font, 30);
	winText.setFillColor(sf::Color::Yellow);
	winText.setPosition(screenSize.x / 2, window.getSize().y / 2);
	winText.setOrigin(winText.getLocalBounds().width / 2, winText.getLocalBounds().height / 2);
	window.draw(winText);
}