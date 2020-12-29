/*
 * Title.cpp
 *
 *  Created on: Dec 29, 2020
 *      Author: private
 */

#include "../include/Title.hpp"

Title::Title() {
	this->textureWithPosition = NULL;
}

Title::~Title() {
	delete (textureWithPosition);
}

void Title::render(SDL_Renderer *renderer) {
	SDL_RenderCopy(renderer, getTextureWithPosition()->getTexture(), NULL,
			&(getTextureWithPosition()->getPosition()));
}

void Title::render(SDL_Renderer *renderer, int score) {
}
TextureWithPosition* Title::getTextureWithPosition() {
	return this->textureWithPosition;
}

void Title::setTextureWithPosition(TextureWithPosition *textureWithPosition) {
	this->textureWithPosition = textureWithPosition;
}

