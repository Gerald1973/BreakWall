#include "../include/Wall002.hpp"
#include <SDL2/SDL.h>
#include "../include/InitUtils.hpp"
#include "../include/ExplodingBrick.hpp"
#include "../include/MissileBrick.hpp"
#include "../include/MovingRotatingBrick.hpp"

void Wall002::init()
{
	std::vector<Brick *> bricks;
	this->background = new Background("resources/images/bad_girl.jpg");
	int brickHeight;
	int brickWidth;
	SDL_QueryTexture(InitUtils::getInstance()->getMapTextures()[GlobalConstants::TEXTURE_KEY], nullptr, nullptr, &brickWidth, &brickHeight);
	int maxNumberOfBricksOnX = (GlobalConstants::WALL_ZONE_WIDTH - GlobalConstants::WALL_ZONE_X) / brickWidth;
	int maxNumberOfBricksOnY = (GlobalConstants::WALL_ZONE_HEIGHT - GlobalConstants::WALL_ZONE_Y) / brickHeight;
	int brickCount = 0;
	for (int y = 0; y < maxNumberOfBricksOnY; y++)
	{
		for (int x = 0; x < maxNumberOfBricksOnX; x++)
		{
			Brick *brick;
			switch (brickCount % 4)
			{
			case 1:brick = new MissileBrick();break;
			case 2:brick = new ExplodingBrick();break;	
			case 3:brick = new MovingRotatingBrick();break;
			default:
				brick = new Brick();
				break;
			}		
			if (brickCount % 3 == 0){
				
			} else
			{
				brick = new ExplodingBrick();
			}
			brick->init();
			brick->getTextureWithPosition()->setX(x * brick->getTextureWithPosition()->getPosition().w);
			brick->getTextureWithPosition()->setY(y * brick->getTextureWithPosition()->getPosition().h);
			bricks.push_back(brick);
			brickCount++;
		}
	}
	setBricks(bricks);
	initSong("Wall002");
}