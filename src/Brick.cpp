#include "../include/Brick.h"
#include "SDL2/SDL.h"
#include "../include/GlobalConstants.h"

Brick::Brick()
{
	this->value = 20;
	this->sound = NULL;
	this->destroyed = false;
	this->textureWithPosition = NULL;
}
void Brick::setValue(int value)
{
	this->value = value;
}

int Brick::getValue()
{
	return this->value;
}

TextureWithPosition *Brick::getTextureWithPosition()
{
	return this->textureWithPosition;
}

void Brick::setTextureWithPosition(TextureWithPosition *textureWithPosition)
{
	this->textureWithPosition = textureWithPosition;
}

Brick::~Brick()
{
	this->setDestroyed(false);
	delete this->textureWithPosition;
}

bool Brick::isDestroyed()
{
	return this->destroyed;
}

void Brick::setDestroyed(bool destroyed)
{
	this->destroyed = destroyed;
}

void Brick::render(SDL_Renderer *renderer)
{
	if (!isDestroyed())
	{
		SDL_RenderCopy(renderer, getTextureWithPosition()->getTexture(), NULL,
					   &(getTextureWithPosition()->getPosition()));
	}
	else
	{
		playDestroy(renderer);
	}
}

void Brick::playDestroy(SDL_Renderer *renderer)
{
	int y = getTextureWithPosition()->getY();
	int h = getTextureWithPosition()->getPosition().h;
	SDL_Rect target = getTextureWithPosition()->getPosition();
	if (y < GlobalConstants::BALL_ZONE_Y + GlobalConstants::BALL_ZONE_HEIGHT - h)
	{
		y = y + 2;
		h--;
		getTextureWithPosition()->setY(y);
		getTextureWithPosition()->setH(h);
		SDL_RenderCopy(renderer, getTextureWithPosition()->getTexture(), NULL,
					   &(getTextureWithPosition()->getPosition()));
	}
}

Mix_Chunk *Brick::getSound()
{
	return this->sound;
}

void Brick::setSound(Mix_Chunk *sound)
{
	this->sound = sound;
}
