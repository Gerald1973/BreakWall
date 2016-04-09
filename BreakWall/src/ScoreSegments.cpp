/*
 * ScoreSegments.cpp
 *
 *  Created on: 9 avr. 2016
 *      Author: Papa
 */

#include "ScoreSegments.h"
#include "UtilConstants.h"
#include <math.h>

ScoreSegments::ScoreSegments() {
	this->posX = 0;
	this->posY = UtilConstants::getInstance()->screenSize.h - this->height;
	this->sdlTexture = NULL;
	this->numberOfDigit = 6;
	this->score = 0;
}

ScoreSegments::~ScoreSegments() {
	// TODO Auto-generated destructor stub
}

void ScoreSegments::render(SDL_Renderer* renderer) {
	render(renderer, this->score);
}

void ScoreSegments::render(SDL_Renderer* renderer, int score) {
	SDL_Rect srcRect;
	SDL_Rect dstRect;
	int tmp = score;
	for (int i = 6; i > 0; i--) {
		if (tmp > 0) {
			srcRect = caluclateSrcRect(tmp % 10);
			tmp = tmp / 10;
		} else {
			srcRect.x = 0;
			srcRect.y = 0;
			srcRect.h = this->height;
			srcRect.w = this->width;
		}
		dstRect.x = (i - 1) * this->width;
		dstRect.y = UtilConstants::getInstance()->screenSize.h - this->height;
		dstRect.w = this->width;
		dstRect.h = this->height;
		SDL_RenderCopy(renderer, this->sdlTexture, &srcRect, &dstRect);
	}
}

void ScoreSegments::setTexture(SDL_Texture* sdlTexture) {
	this->sdlTexture = sdlTexture;
}

SDL_Rect ScoreSegments::caluclateSrcRect(int figure) {
	SDL_Rect result;
	result.x = (figure + 1) * this->width;
	result.y = 0;
	result.w = this->width;
	result.h = this->height;
	return result;
}

void ScoreSegments::addScore(int value) {
	this->score = this->score + value;
}
