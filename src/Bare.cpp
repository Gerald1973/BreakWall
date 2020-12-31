#include "../include/Bare.h"
#include "../include/InitUtils.hpp"
#include "../include/UtilConstants.h"
#include "../include/CustomEventUtils.hpp"

#include <iostream>

Bare::Bare() {
	this->sound = NULL;
	this->textureWithPosition = NULL;
	InitUtils::getInstance()->addTexture("bare.png", TEXTURE_KEY);
	InitUtils::getInstance()->addSoundEffect("bare.wav", SOUND_KEY);
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

