#include "../include/Ball.h"

#include <SDL2/SDL_mixer.h>
#include <cmath> 
#include "../include/CustomEventUtils.hpp"
#include "../include/InitUtils.hpp"
#include "../include/UtilConstants.h"
#include "../include/GlobalConstants.h"

Ball::Ball() {
	this->dirX = 0;
	this->speed = 8;
	this->dirY = 8;
	this->coeffY = 1;
	this->coeffX = 0;
	this->textureWithPosition = nullptr;
	this->posX = 0;
	this->posY = 0;
	InitUtils::getInstance()->addTexture("resources/images/ball.png", TEXTURE_KEY);
}

Ball::~Ball() {
}

void Ball::render() {
	SDL_RenderCopy(InitUtils::getInstance()->getRenderer(), getTextureWithPosition()->getTexture(), NULL, &(getTextureWithPosition()->getPosition()));
}

void Ball::moveBall() {
	bouncesOnScreen();
	//4 Move
	posX = posX + dirX;
	posY = posY + dirY;
	getTextureWithPosition()->setX(round(posX));
	getTextureWithPosition()->setY(round(posY));
	CustomEventUtils::getInstance()->postEventBallMoved(this);
}

TextureWithPosition* Ball::getTextureWithPosition() {
	return this->textureWithPosition;
}

void Ball::setTextureWithPosition(TextureWithPosition *textureWithPosition) {
	this->textureWithPosition = textureWithPosition;
	posX = textureWithPosition->getX();
	posY = textureWithPosition->getY();
}

float Ball::getSpeed() {
	return this->speed;
}

void Ball::setSpeed(float speed) {
	this->speed = speed;
}

bool Ball::bouncesOnScreen() {
	bool result = false;
	int x = this->getTextureWithPosition()->getAbsCenterX();
	int y = this->getTextureWithPosition()->getAbsCenterY();
	int halfBallSize = this->getTextureWithPosition()->getPosition().w / 2;
	//top
	if (y <= UtilConstants::getInstance()->gameZone.y + halfBallSize) {
		dirY = -dirY;
		result = true;
	}
	//right
	if (x >= UtilConstants::getInstance()->gameZone.x + UtilConstants::getInstance()->gameZone.w - halfBallSize) {
		dirX = -abs(dirX);
		result = true;
	}
	//bottom
	if (y >= UtilConstants::getInstance()->gameZone.y + UtilConstants::getInstance()->gameZone.h - halfBallSize) {
		CustomEventUtils::getInstance()->postEventBorderTouched(CustomEventUtils::Code::BORDER_BOTTOM_TOUCHED, this);
	}
	//left
	if (x <= UtilConstants::getInstance()->gameZone.x + halfBallSize) {
		dirX = abs(dirX);
		result = true;
	}
	return result;
}

void Ball::setDirX(float dirX) {
	this->dirX = dirX;
}

float Ball::getDirX() {
	return this->dirX;
}

void Ball::setDirY(float dirY) {
	this->dirY = dirY;
}

float Ball::getDirY() {
	return this->dirY;
}

void Ball::init() {
	SDL_Rect ballRect;
	SDL_Texture *texture = InitUtils::getInstance()->getMapTextures()[Ball::TEXTURE_KEY];
	int widthBall;
	int heightBall;
	SDL_QueryTexture(texture, nullptr, nullptr, &widthBall, &heightBall);
	this->posX = (GlobalConstants::BALL_ZONE_X + GlobalConstants::BALL_ZONE_WIDTH - widthBall) / 2;
	this->posY = GlobalConstants::WALL_ZONE_Y + GlobalConstants::WALL_ZONE_HEIGHT + heightBall;
	this->dirX = 0;
	this->speed = 8;
	this->dirY = 8;
	this->coeffY = 1;
	this->coeffX = 0;
	if (this -> textureWithPosition == nullptr){
		textureWithPosition = new TextureWithPosition(InitUtils::getInstance()->getMapTextures()[Ball::TEXTURE_KEY], ballRect);
	} else {
		textureWithPosition->setX(this->posX);
		textureWithPosition->setY(this->posY);
	}
}

void Ball::setCoeffX(float coeffX) {
	this->coeffX = coeffX;
}

void Ball::setCoeffY(float coeffY) {
	this->coeffY = coeffY;
}

float Ball::getCoeffX() {
	return this->coeffX;
}

float Ball::getCoeffY() {
	return this->coeffY;
}

void Ball::performEvent(SDL_Event &event) {
	switch (event.user.code) {
	case CustomEventUtils::Code::BORDER_BOTTOM_TOUCHED:
		init();
	}
}
