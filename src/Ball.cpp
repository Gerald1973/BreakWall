#include "../include/Ball.h"
#include <SDL2/SDL_mixer.h>
#include "../include/CustomEventUtils.hpp"

Ball::Ball()
{
	this->dirX = 0;
	this->dirY = 2;
	this->speed = 5;
	this->coeffX = 1;
	this->coeffX = 0;
	this->wall = NULL;
	this->textureWithPosition = NULL;
	this->posX = 0;
	this->posY = 0;
	this->bare = NULL;
}

Ball::~Ball()
{
	delete (textureWithPosition);
}

void Ball::render(SDL_Renderer *renderer)
{
	SDL_RenderCopy(renderer, getTextureWithPosition()->getTexture(), NULL,
				   &(getTextureWithPosition()->getPosition()));
}

void Ball::moveBall()
{
	bouncesOnScreen();
	bouncesOnBare(getBare());
	//2 the wall
	std::vector<std::vector<Brick *>> bricks = getWall()->getBricks();
	bool brickTouched = false;
	for (unsigned int j = 0; j < bricks.size(); j++)
	{
		for (unsigned int i = 0; i < bricks[j].size(); i++)
		{
			if (bouncesOnBrick(bricks[j][i]))
			{
				brickTouched = true;
				break;
			}
		}
		if (brickTouched)
		{
			break;
		}
	}
	//4 Move
	posX = posX + dirX;
	posY = posY + dirY;
	getTextureWithPosition()->setX(posX);
	getTextureWithPosition()->setY(posY);
}

Bare *Ball::getBare()
{
	return bare;
}

void Ball::setBare(Bare *bare)
{
	this->bare = bare;
}

TextureWithPosition *Ball::getTextureWithPosition()
{
	return this->textureWithPosition;
}

void Ball::setTextureWithPosition(TextureWithPosition *textureWithPosition)
{
	this->textureWithPosition = textureWithPosition;
	posX = textureWithPosition->getX();
	posY = textureWithPosition->getY();
}

Wall *Ball::getWall()
{
	return wall;
}

void Ball::setWall(Wall *wall)
{
	this->wall = wall;
}

bool Ball::bouncesOnBrick(Brick *brick)
{
	bool result = false;
	if (!brick->isDestroyed() && getTextureWithPosition()->getX2() >= brick->getTextureWithPosition()->getX() && getTextureWithPosition()->getX() <= brick->getTextureWithPosition()->getX2() && getTextureWithPosition()->getY() <= brick->getTextureWithPosition()->getY2() && getTextureWithPosition()->getY2() >= brick->getTextureWithPosition()->getY())
	{
		//Destroy the brick and update the score.
		brick->setDestroyed(true);
		Mix_PlayChannel(-1, brick->getSound(), 0);
		CustomEventUtils::getInstance()->postEventBrickTouched(brick);
		//...

		
		if (getTextureWithPosition()->getAbsCenterX() < brick->getTextureWithPosition()->getX())
		{
			dirX = -dirX;
			posX = brick->getTextureWithPosition()->getX() - getTextureWithPosition()->getPosition().w + dirX;
		}
		else if (getTextureWithPosition()->getAbsCenterX() > brick->getTextureWithPosition()->getX2())
		{
			dirX = -dirX;
			posX = brick->getTextureWithPosition()->getX2() + dirX;
		}
		if (getTextureWithPosition()->getAbsCenterY() < brick->getTextureWithPosition()->getY())
		{
			dirY = -dirY;
			posY = brick->getTextureWithPosition()->getY() - getTextureWithPosition()->getPosition().h + dirY;
		}
		else if (getTextureWithPosition()->getAbsCenterY() > brick->getTextureWithPosition()->getY2())
		{
			dirY = -dirY;
			posY = brick->getTextureWithPosition()->getY2() + dirY;
		}
		result = true;
	}
	return result;
}

float Ball::getSpeed()
{
	return this->speed;
}

void Ball::setSpeed(float speed)
{
	this->speed = speed;
}

bool Ball::bouncesOnScreen()
{
	bool result = false;
	if (posX <= UtilConstants::getInstance()->gameZone.x)
	{
		dirX = -dirX;
		posX = UtilConstants::getInstance()->gameZone.x;
		result = true;
	}
	if (posX + getTextureWithPosition()->getPosition().w >= UtilConstants::getInstance()->gameZone.x + UtilConstants::getInstance()->gameZone.w)
	{
		dirX = -dirX;
		posX = posX - this->getTextureWithPosition()->getPosition().w;
		result = true;
	}
	if (posY <= UtilConstants::getInstance()->gameZone.y)
	{
		dirY = -dirY;
		posY = UtilConstants::getInstance()->gameZone.y;
		result = true;
	}
	if (posY + getTextureWithPosition()->getPosition().h >= UtilConstants::getInstance()->gameZone.y + UtilConstants::getInstance()->gameZone.h)
	{
		dirY = -dirY;
		posY = posY - this->getTextureWithPosition()->getPosition().h;
		result = true;
	}
	return result;
}

bool Ball::bouncesOnBare(Bare *bare)
{
	bool result = false;
	if (
		   getTextureWithPosition()->getX2() >= bare->getTextureWithPosition()->getX() && getTextureWithPosition()->getX() <= bare->getTextureWithPosition()->getX2() 
		&& getTextureWithPosition()->getY2() >= bare->getTextureWithPosition()->getY() && getTextureWithPosition()->getY() <= bare->getTextureWithPosition()->getY2()
	)
	{
		Mix_PlayChannel(-1, bare->getSound(), 0);
		dirY = -dirY;
		//calcul coeffX
		float centerBare =  (float)(bare->getTextureWithPosition()->getAbsCenterX());
		float centerBall =  (float) (getTextureWithPosition()->getAbsCenterX());
		float halfWidht  =  (float) ((bare->getTextureWithPosition()->getPosition().w + getTextureWithPosition()->getPosition().w)/2);
		float deltaCenter = centerBall - centerBare;
		coeffX = deltaCenter / halfWidht;
		coeffy = sqrt(1 - coeffX * coeffX);
		std::cout << "DEBUG Coefficient X = " << coeffX << "\n";
		std::cout << "DEBUG Coefficient Y = " << coeffy << "\n";
		result = true;
		dirX =  coeffX * speed;
		dirY = -coeffy * speed;
		
	}
	return result;
}