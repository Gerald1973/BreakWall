#include <SDL2/SDL_mixer.h>
#include <cmath>
#include <iostream>
#include "../include/Bare.hpp"
#include "../include/InitUtils.hpp"
#include "../include/UtilConstants.h"
#include "../include/CustomEventUtils.hpp"
#include "../include/Ball.h"

Bare::Bare() {
	this->sound = NULL;
	this->textureWithPosition = NULL;
	InitUtils::getInstance()->addTexture("resources/images/bare.png", TEXTURE_KEY);
	InitUtils::getInstance()->addSoundEffect("resources/sound/bare.wav", SOUND_KEY);
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
	SDL_Rect position;
	TextureWithPosition *textureWithPosition = new TextureWithPosition(InitUtils::getInstance()->getMapTextures()[Bare::TEXTURE_KEY], position);
	textureWithPosition->setX((UtilConstants::getInstance()->gameZone.w - textureWithPosition->getPosition().w) / 2);
	textureWithPosition->setY(UtilConstants::getInstance()->gameZone.h - textureWithPosition->getPosition().h);
	setTextureWithPosition(textureWithPosition);
	setSound(InitUtils::getInstance()->getMapSounds()[Bare::SOUND_KEY]);
}

void Bare::performEvent(SDL_Event &event) {
	switch (event.user.code) {
		case CustomEventUtils::Code::BALL_MOVED :
			bounces((Ball*) event.user.data1);
			break;
	}
}

bool Bare::bounces(Ball *ball) {
	bool result = false;
	if (ball->getTextureWithPosition()->getX2() >= this->getTextureWithPosition()->getX()
			&& ball->getTextureWithPosition()->getX() <= this->getTextureWithPosition()->getX2()
			&& ball->getTextureWithPosition()->getY2() >= this->getTextureWithPosition()->getY()
			&& ball->getTextureWithPosition()->getY() <= this->getTextureWithPosition()->getY2()) {
		Mix_PlayChannel(-1, this->getSound(), 0);
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
	}
	return result;
}

