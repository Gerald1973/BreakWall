/*
 * ScoreSegments.cpp
 *
 *  Created on: 9 avr. 2016
 *      Author: Papa
 */

#include "../include/ScoreSegments.hpp"
#include <math.h>
#include "../include/GlobalConstants.h"
#include "../include/InitUtils.hpp"

ScoreSegments::ScoreSegments() {
	this->posX = 0;
	this->posY = GlobalConstants::SCREEN_HEIGHT - GlobalConstants::SEVEN_SEGMENT_HEIGHT;
	this->sdlTexture = NULL;
	this->numberOfDigit = 6;
	this->score = 0;
	InitUtils::getInstance()->addTexture("resources/images/segments_48_64.png", TEXTURE_KEY);
}

ScoreSegments::~ScoreSegments() {
	// TODO Auto-generated destructor stub
}

void ScoreSegments::render() {
	render(this->score);
}

void ScoreSegments::render(int score) {
	SDL_Rect srcRect;
	SDL_Rect dstRect;
	int tmp = score;
	for (int i = numberOfDigit; i > 0; i--) {
		if (tmp > 0) {
			srcRect = calculateSrcRect(tmp % 10);
			tmp = tmp / 10;
		} else {
			srcRect.x = 0;
			srcRect.y = 0;
			srcRect.h = GlobalConstants::SEVEN_SEGMENT_HEIGHT;
			srcRect.w = GlobalConstants::SEVEN_SEGMENT_WIDTH;
		}
		dstRect.x = (GlobalConstants::SCREEN_WIDTH - GlobalConstants::SEVEN_SEGMENT_WIDTH * numberOfDigit) / 2 + (i - 1) * GlobalConstants::SEVEN_SEGMENT_WIDTH;
		dstRect.y = GlobalConstants::SCREEN_HEIGHT - GlobalConstants::SEVEN_SEGMENT_HEIGHT;
		dstRect.w = GlobalConstants::SEVEN_SEGMENT_WIDTH;
		dstRect.h = GlobalConstants::SEVEN_SEGMENT_HEIGHT;
		SDL_RenderCopy(InitUtils::getInstance()->getRenderer(), this->sdlTexture, &srcRect, &dstRect);
	}
}

void ScoreSegments::setTexture(SDL_Texture *sdlTexture) {
	this->sdlTexture = sdlTexture;
}

SDL_Rect ScoreSegments::calculateSrcRect(int figure) {
	SDL_Rect result;
	result.x = (figure + 1) * GlobalConstants::SEVEN_SEGMENT_WIDTH;
	result.y = 0;
	result.w = GlobalConstants::SEVEN_SEGMENT_WIDTH;
	result.h = GlobalConstants::SEVEN_SEGMENT_HEIGHT;
	return result;
}

void ScoreSegments::addScore(int value) {
	this->score = this->score + value;
}

void ScoreSegments::init() {
	setTexture(InitUtils::getInstance()->getMapTextures()[ScoreSegments::TEXTURE_KEY]);
}
