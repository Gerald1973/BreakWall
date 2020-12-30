#include "../include/Ball.h"

#include <SDL2/SDL_mixer.h>
#include <cmath> 
#include "../include/CustomEventUtils.hpp"
#include "../include/InitUtils.hpp"

Ball::Ball()
{
	this->dirX = 0;
	this->speed = 8;
	this->dirY = 8;
	this->coeffy = 1;
	this->coeffX = 0;
	this->wall = NULL;
	this->textureWithPosition = NULL;
	this->posX = 0;
	this->posY = 0;
	this->bare = NULL;
	InitUtils::getInstance()->addTexture("ball.png",TEXTURE_KEY);
}

Ball::~Ball()
{
	delete textureWithPosition;
	delete wall;
	delete bare;
}

void Ball::render()
{
	SDL_RenderCopy(InitUtils::getInstance()->getRenderer(), getTextureWithPosition()->getTexture(), NULL,
				   &(getTextureWithPosition()->getPosition()));
}

void Ball::moveBall()
{
	bouncesOnScreen();
	bouncesOnBare(getBare());
	//2 the wall
	std::vector<std::vector<Brick *>> bricks = getWall()->getBricks();
	Brick *touchedBrick = NULL;
	for (unsigned int j = 0; j < bricks.size(); j++)
	{
		for (unsigned int i = 0; i < bricks[j].size(); i++)
		{
			if (isBrickTouched(bricks[j][i]))
			{
				if (!touchedBrick) {
					touchedBrick = bricks[j][i];
				}
				CustomEventUtils::getInstance()->postEventBrickTouched(bricks[j][i]);
				bricks[j][i]->setDestroyed(true);
			}
		}
	}
	//3 bouncing if any
	if (touchedBrick){
		this->bouncesOnBrick(touchedBrick);
	}
	//4 Move
	posX = posX + dirX;
	posY = posY + dirY;
	getTextureWithPosition()->setX(round(posX));
	getTextureWithPosition()->setY(round(posY));
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

void Ball::bouncesOnBrick(Brick* brick){
	Mix_PlayChannel(-1, brick->getSound(), 0);
	int halfBallSize = this->getTextureWithPosition()->getPosition().w / 2;
	int x = this->getTextureWithPosition()->getAbsCenterX();
	int y = this->getTextureWithPosition()->getAbsCenterY();
	if ( x >= brick->getTextureWithPosition()->getX()-halfBallSize && x < brick->getTextureWithPosition()->getX2() + halfBallSize){
		if (
			(y >= brick->getTextureWithPosition()->getY()-halfBallSize && y <= brick->getTextureWithPosition()->getY()) ||
			(y >= brick->getTextureWithPosition()->getY2() && y <= brick->getTextureWithPosition()->getY2() + halfBallSize)
		){
			dirY = -dirY;
		}
	}
	if ( y >= brick->getTextureWithPosition()->getY()-halfBallSize && y <= brick->getTextureWithPosition()->getY2() + halfBallSize){
		if ( 
			(x >= brick->getTextureWithPosition()->getX() - halfBallSize && x <= brick->getTextureWithPosition()->getX()) ||
			(x >= brick->getTextureWithPosition()->getX2() && x <= brick->getTextureWithPosition() -> getX2() + halfBallSize)
		){
			dirX = -dirX;
		}
	}
}

bool Ball::isBrickTouched(Brick *brick)
{
	bool result = false;
	if (!brick->isDestroyed())
	{
		int x = this->getTextureWithPosition()->getAbsCenterX();
		int y = this->getTextureWithPosition()->getAbsCenterY();
		int halfBallSize = this->getTextureWithPosition()->getPosition().w / 2;
		result =  x >= brick->getTextureWithPosition()->getX() - halfBallSize 
		       && x <= brick->getTextureWithPosition()->getX2() + halfBallSize
			   && y >= brick->getTextureWithPosition()->getY() - halfBallSize
			   && y <=  brick->getTextureWithPosition()->getY2() + halfBallSize;
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
	int x = this->getTextureWithPosition()->getAbsCenterX();
	int y = this->getTextureWithPosition()->getAbsCenterY();
	int halfBallSize = this->getTextureWithPosition()->getPosition().w / 2;
	//top
	if (y <= UtilConstants::getInstance() -> gameZone.y + halfBallSize)
	{
		dirY = -dirY;
		result = true;
	}
	//right
	if (x >= UtilConstants::getInstance() -> gameZone.x + UtilConstants::getInstance() -> gameZone.w - halfBallSize)
	{
		dirX = -abs(dirX);
		result = true;
	}
	//bottom
	if (y >= UtilConstants::getInstance() -> gameZone.y + UtilConstants::getInstance() -> gameZone.h - halfBallSize)
	{
		dirY = -dirY;
		result = true;
		
	}
	//left
	if (x <= UtilConstants::getInstance() -> gameZone.x + halfBallSize)
	{
		dirX = abs(dirX);
		result = true;
	}
	return result;
}

bool Ball::bouncesOnBare(Bare *bare)
{
	bool result = false;
	if (
		getTextureWithPosition()->getX2() >= bare->getTextureWithPosition()->getX() && getTextureWithPosition()->getX() <= bare->getTextureWithPosition()->getX2() && getTextureWithPosition()->getY2() >= bare->getTextureWithPosition()->getY() && getTextureWithPosition()->getY() <= bare->getTextureWithPosition()->getY2())
	{
		Mix_PlayChannel(-1, bare->getSound(), 0);
		dirY = -dirY;
		//calcul coeffX
		float centerBare = (float)(bare->getTextureWithPosition()->getAbsCenterX());
		float centerBall = (float)(getTextureWithPosition()->getAbsCenterX());
		float halfWidht = (float)((bare->getTextureWithPosition()->getPosition().w + getTextureWithPosition()->getPosition().w) / 2);
		float deltaCenter = centerBall - centerBare;
		coeffX = deltaCenter / halfWidht;
		coeffy = sqrt(1 - coeffX * coeffX);
		std::cout << "DEBUG Coefficient X = " << coeffX << "\n";
		std::cout << "DEBUG Coefficient Y = " << coeffy << "\n";
		result = true;
		dirX = coeffX * speed;
		dirY = -coeffy * speed;
	}
	return result;
}

void Ball::init(Wall *wall, Bare *bare) {
	SDL_Rect rect;
	rect.x = UtilConstants::getInstance()->gameZone.w / 2 + UtilConstants::getInstance()->gameZone.x;
	rect.y = (GlobalConstants::MAX_NUMBER_OF_BRICKS_ON_Y + 1) * GlobalConstants::BRICK_HEIGHT + UtilConstants::getInstance()->gameZone.y;
	TextureWithPosition *textureWithPosition = new TextureWithPosition(InitUtils::getInstance()->getMapTextures()[Ball::TEXTURE_KEY], rect);
	setTextureWithPosition(textureWithPosition);
	setBare(bare);
	setWall(wall);
}
