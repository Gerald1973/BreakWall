#include "../include/TextureWithPosition.h"

#include <iostream>
#include <SDL2/SDL.h>

#include "../include/UtilConstants.h"

TextureWithPosition::TextureWithPosition(SDL_Texture *texture, SDL_Rect position)
{
    this->position = position;
    this->texture = texture;
    int width = 0;
    int height = 0;
    SDL_QueryTexture(this->getTexture(), NULL, NULL, &width, &height);
    this->originRect.w = width;
    this->originRect.h = height;
    this->originRect.x = 0;
    this->originRect.y = 0;
    this->position.w = width;
    this->position.h = height;
}

TextureWithPosition::TextureWithPosition(SDL_Texture *texture, int x, int y, int width, int height)
{
    SDL_Rect position;
    position.h = 0;
    position.w = 0;
    position.x = 0;
    position.y = 0;
    this->position = position;
    this->texture = texture;
    int _width = 0;
    int _height = 0;
    SDL_QueryTexture(this->getTexture(), NULL, NULL, &_width, &_height);
    this->originRect.w = _width;
    this->originRect.h = _height;
    this->originRect.x = 0;
    this->originRect.y = 0;
    this->position.x = x;
    this->position.y = y;
    this->position.w = width;
    this->position.h = height;
}

TextureWithPosition::~TextureWithPosition()
{
    SDL_DestroyTexture(this->texture);
}

const SDL_Rect &TextureWithPosition::getPosition()
{
    return this->position;
}

const SDL_Rect &TextureWithPosition::getOriginRect()
{
    return this->originRect;
}

void TextureWithPosition::setX(int x)
{
    position.x = x;
    if (position.x < UtilConstants::getInstance()->gameZone.x)
    {
        position.x = UtilConstants::getInstance()->gameZone.x;
    }
    else if (position.x > UtilConstants::getInstance()->gameZone.w - position.w)
    {
        position.x = UtilConstants::getInstance()->gameZone.w - position.w;
    }
}

void TextureWithPosition::setY(int y)
{
    position.y = y;
    // if (position.y < UtilConstants::getInstance()->gameZone.y)
    // {
    //     position.y = UtilConstants::getInstance()->gameZone.y;
    // }
    // else if (position.y > UtilConstants::getInstance()->gameZone.h)
    // {
    //     position.y = UtilConstants::getInstance()->gameZone.h - position.h;
    // }
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
/**
 * @brief This method returns the position of the center of the elements in the main panel on the x axis.
 * 
 * @return int the position of the center of the elements in the main panel on the x axis.
 */
int TextureWithPosition::getAbsCenterX()
{
    return (position.w / 2) + position.x;
}

/**
 * @brief This method returns the position of the center of the elements in the main panel on the y axis.
 * 
 * @return int int the position of the center of the elements in the main panel on the y axis.
 */
int TextureWithPosition::getAbsCenterY()
{
    return (position.h / 2) + position.y;
}

SDL_Texture *TextureWithPosition::getTexture()
{

    return texture;
}

void TextureWithPosition::setTexture(SDL_Texture *sdlTexture)
{
    texture = sdlTexture;
}

void TextureWithPosition::setW(int w){
    this->position.w = w;
}

void TextureWithPosition::setH(int h){
    this->position.h = h;
}
