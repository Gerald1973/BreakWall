#include "../include/Background.hpp"
#include "../include/InitUtils.hpp"

Background::Background()
{
    this->textureWithPosition = NULL;
    InitUtils::getInstance()->addTexture("winter_01.jpg", TEXTURE_KEY);
}

Background::~Background()
{
    delete (textureWithPosition);
}

void Background::render()
{
    SDL_RenderCopy(InitUtils::getInstance()->getRenderer(), getTextureWithPosition()->getTexture(), NULL,
                   &(getTextureWithPosition()->getPosition()));
}

TextureWithPosition* Background::getTextureWithPosition() {
	return this->textureWithPosition;
}

void Background::setTextureWithPosition(TextureWithPosition* textureWithPosition) {
	this->textureWithPosition = textureWithPosition;
}
