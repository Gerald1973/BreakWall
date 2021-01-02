#include "../include/Background.hpp"
#include "../include/InitUtils.hpp"
#include "../include/UtilConstants.h"

Background::Background() {
	this->textureWithPosition = NULL;
	InitUtils::getInstance()->addTexture("resources/images/winter_01.jpg", TEXTURE_KEY);
}

Background::~Background() {
	delete (textureWithPosition);
}

void Background::render() {
	SDL_RenderCopy(InitUtils::getInstance()->getRenderer(), getTextureWithPosition()->getTexture(), NULL, &(getTextureWithPosition()->getPosition()));
}

TextureWithPosition* Background::getTextureWithPosition() {
	return this->textureWithPosition;
}

void Background::setTextureWithPosition(TextureWithPosition *textureWithPosition) {
	this->textureWithPosition = textureWithPosition;
}

void Background::init() {
	TextureWithPosition *textureWithPosition = new TextureWithPosition(InitUtils::getInstance()->getMapTextures()[Background::TEXTURE_KEY],
			UtilConstants::getInstance()->gameZone.x, UtilConstants::getInstance()->gameZone.y, UtilConstants::getInstance()->gameZone.w,
			UtilConstants::getInstance()->gameZone.h);
	setTextureWithPosition(textureWithPosition);
}
