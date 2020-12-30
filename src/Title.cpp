/*
 * Title.cpp
 *
 *  Created on: Dec 29, 2020
 *      Author: private
 */

#include "../include/Title.hpp"
#include "../include/InitUtils.hpp"
#include "../include/GlobalConstants.h"

Title::Title() {
	this->textureWithPosition = NULL;
	InitUtils::getInstance()->addTexture("breakwall_title.png", TEXTURE_KEY);
}

Title::~Title() {
	delete (textureWithPosition);
}

void Title::render() {
	SDL_RenderCopy(InitUtils::getInstance()->getRenderer(), getTextureWithPosition()->getTexture(), NULL, &(getTextureWithPosition()->getPosition()));
}

TextureWithPosition* Title::getTextureWithPosition() {
	return this->textureWithPosition;
}

void Title::init() {
	TextureWithPosition *textureWithPosition = new TextureWithPosition(InitUtils::getInstance()->getMapTextures()[Title::TEXTURE_KEY], 0, 0,
			GlobalConstants::SCREEN_WIDTH, GlobalConstants::SCREEN_HEIGHT);
	setTextureWithPosition(textureWithPosition);
}

void Title::setTextureWithPosition(TextureWithPosition *textureWithPosition) {
	this->textureWithPosition = textureWithPosition;
}

