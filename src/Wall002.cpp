#include "../include/Wall002.hpp"
#include <SDL2/SDL.h>
#include "../include/InitUtils.hpp"

void Wall002::init() {
	std::vector<Brick*> bricks;
	this->background = new Background("resources/images/bad_girl.jpg");
	int brickHeight;
		int brickWidth;
		SDL_QueryTexture(InitUtils::getInstance()->getMapTextures()[GlobalConstants::TEXTURE_KEY], nullptr, nullptr, &brickWidth, &brickHeight);
		int maxNumberOfBricksOnX = (GlobalConstants::WALL_ZONE_WIDTH - GlobalConstants::WALL_ZONE_X) / brickWidth;
		int maxNumberOfBricksOnY = (GlobalConstants::WALL_ZONE_HEIGHT - GlobalConstants::WALL_ZONE_Y) / brickHeight;
		for (int y = 0; y < maxNumberOfBricksOnY; y = y + 2) {
			for (int x = 0; x < maxNumberOfBricksOnX; x++) {
				Brick *brick = new Brick();
				brick->init();
				brick->getTextureWithPosition()->setX(x * brick->getTextureWithPosition()->getPosition().w);
				brick->getTextureWithPosition()->setY(y * brick->getTextureWithPosition()->getPosition().h);
				bricks.push_back(brick);
			}
		}
	setBricks(bricks);
	initSong("Wall002");
}
