#include "../include/Background.h"

Background::Background()
{
    this->textureWithPosition = NULL;
}

Background::~Background()
{
    delete (textureWithPosition);
}

void Background::render(SDL_Renderer *renderer)
{
    SDL_RenderCopy(renderer, getTextureWithPosition()->getTexture(), NULL,
                   &(getTextureWithPosition()->getPosition()));
}

TextureWithPosition* Background::getTextureWithPosition() {
	return this->textureWithPosition;
}

void Background::setTextureWithPosition(TextureWithPosition* textureWithPosition) {
	this->textureWithPosition = textureWithPosition;
}