#include "../include/Brick.h"
#include "SDL2/SDL.h"
#include "../include/InitUtils.hpp"
#include "../include/CustomEventUtils.hpp"
#include "../include/GlobalConstants.h"
#include <SDL2/SDL_mixer.h>

Brick::Brick() {
	this->value = 20;
	this->sound = NULL;
	this->destroyed = false;
	this->textureWithPosition = NULL;
	this->deadDirection = 1;
	InitUtils::getInstance()->addTexture("brickRed.bmp", TEXTURE_KEY);
	InitUtils::getInstance()->addSoundEffect("metal.wav", SOUND_KEY);
}
void Brick::setValue(int value) {
	this->value = value;
}

int Brick::getValue() {
	return this->value;
}

TextureWithPosition* Brick::getTextureWithPosition() {
	return this->textureWithPosition;
}

void Brick::setTextureWithPosition(TextureWithPosition *textureWithPosition) {
	this->textureWithPosition = textureWithPosition;
}

Brick::~Brick() {
	this->setDestroyed(false);
	delete this->textureWithPosition;
}

bool Brick::isDestroyed() {
	return this->destroyed;
}

void Brick::setDestroyed(bool destroyed) {
	this->destroyed = destroyed;
}

void Brick::render() {
	if (!isDestroyed()) {
		SDL_RenderCopy(InitUtils::getInstance()->getRenderer(), getTextureWithPosition()->getTexture(), NULL, &(getTextureWithPosition()->getPosition()));
	} else {
		playDestroy();
	}
}

void Brick::playDestroy() {

	int y = getTextureWithPosition()->getY();
	int h = getTextureWithPosition()->getPosition().h;
	if (y < GlobalConstants::BALL_ZONE_Y + GlobalConstants::BALL_ZONE_HEIGHT - h) {
		if (h == 0 || h == getTextureWithPosition()->getOriginRect().h) {
			deadDirection = -deadDirection;
		}
		y = y + 2;
		h = h + deadDirection;
		getTextureWithPosition()->setY(y);
		getTextureWithPosition()->setH(h);
		SDL_RenderCopy(InitUtils::getInstance()->getRenderer(), getTextureWithPosition()->getTexture(), NULL, &(getTextureWithPosition()->getPosition()));
	}
}

Mix_Chunk* Brick::getSound() {
	return this->sound;
}

void Brick::setSound(Mix_Chunk *sound) {
	this->sound = sound;
}

bool Brick::isTouchedByBall(Ball *ball) {
	bool result = false;
	if (!isDestroyed()) {
		int x = ball->getTextureWithPosition()->getAbsCenterX();
		int y = ball->getTextureWithPosition()->getAbsCenterY();
		int halfBallSize = ball->getTextureWithPosition()->getPosition().w / 2;
		result = x >= this->getTextureWithPosition()->getX() - halfBallSize && x <= this->getTextureWithPosition()->getX2() + halfBallSize
				&& y >= this->getTextureWithPosition()->getY() - halfBallSize && y <= this->getTextureWithPosition()->getY2() + halfBallSize;
	}
	return result;
}

void Brick::bounces(Ball *ball) {
	Mix_PlayChannel(-1, getSound(), 0);
	int halfBallSize = this->getTextureWithPosition()->getPosition().w / 2;
	int x = ball->getTextureWithPosition()->getAbsCenterX();
	int y = ball->getTextureWithPosition()->getAbsCenterY();
	if (x >= getTextureWithPosition()->getX() - halfBallSize && x < getTextureWithPosition()->getX2() + halfBallSize) {
		if ((y >= getTextureWithPosition()->getY() - halfBallSize && y <= getTextureWithPosition()->getY())
				|| (y >= getTextureWithPosition()->getY2() && y <= getTextureWithPosition()->getY2() + halfBallSize)) {
			ball->setDirY(-ball->getDirY());
		}
	}
	if (y >= getTextureWithPosition()->getY() - halfBallSize && y <= getTextureWithPosition()->getY2() + halfBallSize) {
		if ((x >= getTextureWithPosition()->getX() - halfBallSize && x <= getTextureWithPosition()->getX())
				|| (x >= getTextureWithPosition()->getX2() && x <= getTextureWithPosition()->getX2() + halfBallSize)) {
			ball->setDirX(-ball->getDirX());
		}
	}
}

void Brick::init() {
	setSound(InitUtils::getInstance()->getMapSounds()[Brick::SOUND_KEY]);
	SDL_Rect tmpRect;
	TextureWithPosition *textureWithPosition = new TextureWithPosition(InitUtils::getInstance()->getMapTextures()[Brick::TEXTURE_KEY], tmpRect);
	setTextureWithPosition(textureWithPosition);
}

void Brick::performEvent(SDL_Event &event) {
	if (event.user.code == CustomEventUtils::Code::BALL_MOVED) {
		Ball *ball = (Ball*) event.user.data1;
		if (this->isTouchedByBall(ball)){
			bounces(ball);
			CustomEventUtils::getInstance()->postEventBrickTouched(this);
		}
	}
}
