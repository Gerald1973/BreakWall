#include "../include/Bare.h"
#include "../include/InitUtils.hpp"

#include <iostream>

Bare::Bare()
{
    this->sound = NULL;
    this->textureWithPosition = NULL;
    InitUtils::getInstance()->addTexture("bare.png",TEXTURE_KEY);
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

void Bare::render()
{
    SDL_RenderCopy(InitUtils::getInstance()->getRenderer(), this->textureWithPosition->getTexture(), NULL, &(this->textureWithPosition->getPosition()));
}

Mix_Chunk *Bare::getSound()
{
    return sound;
}

void Bare::setSound(Mix_Chunk *sound)
{
    this->sound = sound;
}
