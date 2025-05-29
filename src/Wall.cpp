#include "../include/Wall.hpp"

#include <SDL2/SDL.h>

#include "../include/Brick.h"
#include "../include/GlobalConstants.h"
#include "../include/UtilConstants.h"
#include "../include/InitUtils.hpp"
#include "../include/CustomEventUtils.hpp"
#include "../include/MovingRotatingBrick.hpp"
#include "../include/ExplodingBrick.hpp"
#include <iostream>

Wall::Wall()
{
}

Wall::~Wall()
{
	std::cout << "Wall destruction." << std::endl;
	delete background;
	background = nullptr;
	bricks.clear();
	bricks.shrink_to_fit();
}

void Wall::render()
{
	this->background->render();
	for (unsigned int i = 0; i < bricks.size(); i++)
	{
		bricks[i]->render();
	}
}

std::vector<Brick *> Wall::getBricks()
{
	return this->bricks;
}

void Wall::setBricks(std::vector<Brick *> bricks)
{
	this->bricks = bricks;
}

void Wall::performEvent(SDL_Event &event)
{
	if (event.user.code == CustomEventUtils::Code::BALL_MOVED)
	{
		for (unsigned int i = 0; i < this->bricks.size(); i++)
		{
			Ball *ball = (Ball *)event.user.data1;
			if (this->bricks[i]->isTouchedByBall(ball))
			{
				this->bricks[i]->performEvent(event);
				break;
			}
		}
	}
	else if (event.user.code == CustomEventUtils::Code::BRICK_REMOVED)
	{
		Brick *brick = (Brick *)event.user.data1;
		int index = this->findIndex(brick);
		this->bricks.erase(bricks.begin() + index);
		delete brick;
		CustomEventUtils::getInstance()->postEventBrickRemaining(this);
	}
}

int Wall::findIndex(Brick *brick)
{
	int result = -1;
	if (this->bricks.size() > 0)
	{
		int size = (int)bricks.size();
		for (int i = 0; i < size; i++)
		{
			if (bricks[i] == brick)
			{
				result = i;
				break;
			}
		}
	}
	return result;
}

void Wall::init()
{
	std::vector<Brick *> bricks;
	int brickHeight;
	int brickWidth;
	background = new Background("resources/images/donald_trump.jpg");
	SDL_QueryTexture(InitUtils::getInstance()->getMapTextures()[GlobalConstants::TEXTURE_KEY], nullptr, nullptr, &brickWidth, &brickHeight);
	int maxNumberOfBricksOnX = (GlobalConstants::WALL_ZONE_WIDTH - GlobalConstants::WALL_ZONE_X) / brickWidth;
	int maxNumberOfBricksOnY = (GlobalConstants::WALL_ZONE_HEIGHT - GlobalConstants::WALL_ZONE_Y) / brickHeight;
	int brickCounter = 0;
	for (int y = 0; y < maxNumberOfBricksOnY; y++)
	{
		int x = y % 2;
		for (; x < maxNumberOfBricksOnX; x = x + 2)
		{
			Brick *brick = NULL;
			switch (brickCounter % 3){
			case 1: brick = new MovingRotatingBrick();break;
			case 2: brick = new ExplodingBrick();break;
			default:
				brick = new Brick();
				break;
			}
			brick->init();
			brick->getTextureWithPosition()->setX(x * brick->getTextureWithPosition()->getPosition().w);
			brick->getTextureWithPosition()->setY(y * brick->getTextureWithPosition()->getPosition().h);
			bricks.push_back(brick);
			brickCounter++;
		}
	}
	setBricks(bricks);
	initSong("Wall");
}

std::string Wall::getSongFileName()
{
	return this->songFileName;
}

std::string Wall::getSongKey()
{
	return this->songKey;
}

void Wall::setSongFileName(std::string songFileName)
{
	this->songFileName = songFileName;
}

void Wall::setSongKey(std::string songKey)
{
	this->songKey = songKey;
}

void Wall::playSong()
{
}

void Wall::resetSong()
{
	Mix_RewindMusic();
}

void Wall::initSong(std::string key)
{
	Mix_PlayMusic(InitUtils::getInstance()->getMixMusic(key), -1);
}

void Wall::stopSong()
{
	Mix_HaltMusic();
}
