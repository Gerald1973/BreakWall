#include "../include/Bare.h"
#include <iostream>

Bare::Bare()
{
    this->sound = NULL;
    this->textureWithPosition = NULL;
}

Bare::~Bare()
{
    delete (this->textureWithPosition);
}

void Bare::setTextureWithPosition(TextureWithPosition *textureWithPosition)
{
    this->textureWithPosition = textureWithPosition;
}

TextureWithPosition *Bare::getTextureWithPosition()
{
    return this->textureWithPosition;
}

void Bare::render(SDL_Renderer *renderer)
{
    SDL_RenderCopy(renderer, this->textureWithPosition->getTexture(), NULL, &(this->textureWithPosition->getPosition()));
}

Mix_Chunk *Bare::getSound()
{
    return sound;
}

void Bare::setSound(Mix_Chunk *sound)
{
    this->sound = sound;
}