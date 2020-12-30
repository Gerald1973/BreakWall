/*
 * Title.cpp
 *
 *  Created on: Dec 29, 2020
 *      Author: private
 */

#include "../include/Title.hpp"
#include "../include/InitUtils.hpp"

Title::Title() {
	this->textureWithPosition = NULL;
}

Title::~Title() {
	delete (textureWithPosition);
}

void Title::render() {
	SDL_RenderCopy(InitUtils::getInstance()->getRenderer(), getTextureWithPosition()->getTexture(), NULL,
			&(getTextureWithPosition()->getPosition()));
}

TextureWithPosition* Title::getTextureWithPosition() {
	return this->textureWithPosition;
}

void Title::setTextureWithPosition(TextureWithPosition *textureWithPosition) {
	this->textureWithPosition = textureWithPosition;
}

