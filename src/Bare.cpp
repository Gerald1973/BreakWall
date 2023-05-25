#include <SDL2/SDL_mixer.h>
#include <cmath>
#include <iostream>
#include "../include/Bare.hpp"
#include "../include/InitUtils.hpp"
#include "../include/CustomEventUtils.hpp"
#include "../include/Ball.h"
#include "../include/GlobalConstants.h"

Bare::Bare() {
	this->sound = nullptr;
	this->textureWithPosition = nullptr;
	InitUtils::getInstance()->addTexture("resources/images/bare.png", TEXTURE_KEY);
}

Bare::~Bare() {
	delete (this->textureWithPosition);
}

void Bare::setTextureWithPosition(TextureWithPosition *textureWithPosition) {
	this->textureWithPosition = textureWithPosition;
}

TextureWithPosition* Bare::getTextureWithPosition() {
	return this->textureWithPosition;
}

void Bare::render() {
	SDL_RenderCopy(InitUtils::getInstance()->getRenderer(), this->textureWithPosition->getTexture(), NULL, &(this->textureWithPosition->getPosition()));
}

Mix_Chunk* Bare::getSound() {
	return sound;
}

void Bare::setSound(Mix_Chunk *sound) {
	this->sound = sound;
}

void Bare::init() {
	int widhtBare;
	int heightBare;
	SDL_QueryTexture(InitUtils::getInstance()->getMapTextures()[TEXTURE_KEY], nullptr, nullptr, &widhtBare, &heightBare);
	int posX = (GlobalConstants::BALL_ZONE_X + GlobalConstants::BALL_ZONE_WIDTH - widhtBare) / 2;
	int posY = GlobalConstants::BALL_ZONE_Y + GlobalConstants::BALL_ZONE_HEIGHT - heightBare;
	if (textureWithPosition == nullptr) {
		this->textureWithPosition = new TextureWithPosition(InitUtils::getInstance()->getMapTextures()[TEXTURE_KEY], posX, posY, widhtBare, heightBare);
	} else {
		this->textureWithPosition->setX(posX);
		this->textureWithPosition->setY(posY);
	}
	setSound(InitUtils::getInstance()->getMapSounds()[Bare::SOUND_KEY]);
}

void Bare::performEvent(SDL_Event &event) {
	switch (event.type) {
	case SDL_MOUSEMOTION:
		getTextureWithPosition()->setX(getTextureWithPosition()->getX() + event.motion.xrel);
		break;
	case SDL_USEREVENT:
		switch (event.user.code) {
		case CustomEventUtils::Code::BALL_MOVED:
			bounces((Ball*) event.user.data1);
			break;
		case CustomEventUtils::Code::BORDER_BOTTOM_TOUCHED:
			init();
			break;
		}
		break;
	}
}

bool Bare::bounces(Ball *ball) {
	bool result = false;
	if (ball->getTextureWithPosition()->getX2() >= this->getTextureWithPosition()->getX()
			&& ball->getTextureWithPosition()->getX() <= this->getTextureWithPosition()->getX2()
			&& ball->getTextureWithPosition()->getY2() >= this->getTextureWithPosition()->getY()
			&& ball->getTextureWithPosition()->getY() <= this->getTextureWithPosition()->getY2()) {
		if (!ball->isGlued()) {
			Mix_PlayChannel(-1,InitUtils::getInstance()->getMapSounds()[GlobalConstants::BARE_SOUND_KEY], 0);
			float centerBare = this->getTextureWithPosition()->getAbsCenterX();
			float centerBall = ball->getTextureWithPosition()->getAbsCenterX();
			float halfWidhtBare = (this->getTextureWithPosition()->getPosition().w + ball->getTextureWithPosition()->getPosition().w) / 2;
			float deltaCenter = centerBall - centerBare;
			ball->setCoeffX(deltaCenter / halfWidhtBare);
			float tmpCoeffY = sqrt(1 - ball->getCoeffX() * ball->getCoeffX());
			ball->setCoeffY(tmpCoeffY);
			std::cout << "DEBUG Coefficient X = " << ball->getCoeffX() << std::endl;
			std::cout << "DEBUG Coefficient Y = " << ball->getCoeffY() << std::endl;
			result = true;
			ball->setDirX(ball->getCoeffX() * ball->getSpeed());
			ball->setDirY(-ball->getCoeffY() * ball->getSpeed());
			CustomEventUtils::getInstance()->postEventBareTouched();
		} else {
			ball->setDirX(0);
			ball->setDirY(0);
		}
	}
	return result;
}
