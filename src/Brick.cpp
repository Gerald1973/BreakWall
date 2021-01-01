#include "../include/Brick.h"
#include "SDL2/SDL.h"
#include "../include/InitUtils.hpp"
#include "../include/CustomEventUtils.hpp"
#include "../include/GlobalConstants.h"

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

void Brick::init() {
	setSound(InitUtils::getInstance()->getMapSounds()[Brick::SOUND_KEY]);
	SDL_Rect tmpRect;
	TextureWithPosition *textureWithPosition = new TextureWithPosition(InitUtils::getInstance()->getMapTextures()[Brick::TEXTURE_KEY], tmpRect);
	setTextureWithPosition(textureWithPosition);
}

void Brick::performEvent(SDL_Event &event) {
	if (event.user.code == CustomEventUtils::Code::BALL_MOVED) {
		std::cout << "To do act her..." << std::endl;
	}
}
