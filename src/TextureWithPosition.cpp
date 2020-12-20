#include "../include/TextureWithPosition.h"
#include <iostream>
#include <SDL2/SDL.h>
#include "../include/UtilConstants.h"

TextureWithPosition::TextureWithPosition(SDL_Texture* texture, SDL_Rect position)
{
    this->position=position;
    this->texture=texture;
    int width = 0;
    int height = 0;
    SDL_QueryTexture(this->getTexture(),NULL,NULL,&width,&height);
    this->originRect.w = width;
    this->originRect.h = height;
    this->position.w = width;
    this->position.h = height;
    //std::cout << "Origin w:" << this->originRect.w << "\n";
    //std::cout << "Origin h:" << this->originRect.h  <<"\n";
    //std::cout << "Origin x:" << this->originRect.x  <<"\n";
    //std::cout << "Origin y:" << this->originRect.y  <<"\n";
}

TextureWithPosition::~TextureWithPosition()
{
    SDL_DestroyTexture(this->texture);
}

const SDL_Rect & TextureWithPosition::getPosition()
{
    return this->position;
}

const SDL_Rect & TextureWithPosition::getOriginRect(){
    return this->originRect;
}

void TextureWithPosition::setX(int x)
{
    position.x = x;
    if (position.x < UtilConstants::getInstance()->screenSize.x)
    {
        position.x = UtilConstants::getInstance()->screenSize.x;
    }
    else if (position.x > UtilConstants::getInstance()->screenSize.w - position.w)
    {
        position.x = UtilConstants::getInstance()->screenSize.w - position.w;
    }
}

void TextureWithPosition::setY(int y)
{
    position.y = y;
    if (position.y < UtilConstants::getInstance()->screenSize.y)
    {
        position.y = UtilConstants::getInstance()->screenSize.y;
    }
    else if (position.y > UtilConstants::getInstance()->screenSize.h)
    {
        position.y = UtilConstants::getInstance()->screenSize.h - position.h;
    }
}

int TextureWithPosition::getX()
{
    return position.x;
}

int TextureWithPosition::getY()
{
    return position.y;
}

int TextureWithPosition::getX2()
{
    return position.x + position.w;
}

int TextureWithPosition::getY2()
{
    return position.y + position.h;
}

int TextureWithPosition::getAbsCenterX(){
    return (position.w / 2) + position.x;
}

int TextureWithPosition::getAbsCenterY(){
    return (position.h / 2) + position.y;
}



SDL_Texture* TextureWithPosition::getTexture()
{

    return texture;
}

void TextureWithPosition::setTexture(SDL_Texture* sdlTexture)
{
    texture = sdlTexture;
}
