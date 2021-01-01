#include "../include/Wall.hpp"

#include <SDL2/SDL.h>

#include "../include/Brick.h"
#include "../include/GlobalConstants.h"
#include "../include/UtilConstants.h"
#include "../include/InitUtils.hpp"
#include "../include/CustomEventUtils.hpp"

Wall::Wall() {
}

Wall::~Wall() {
}

void Wall::build() {
}

void Wall::render() {
	for (unsigned int i = 0; i < bricks.size(); i++) {
		bricks[i]->render();
	}
}

std::vector<Brick*> Wall::getBricks() {
	return this->bricks;
}

void Wall::setBricks(std::vector<Brick*> bricks) {
	this->bricks = bricks;
}



void Wall::performEvent(SDL_Event &event) {
	if (event.user.code == CustomEventUtils::Code::BALL_MOVED) {
		for (unsigned int i = 0; i < this->bricks.size(); i++) {
			Ball *ball = (Ball*) event.user.data1;
			if (this->bricks[i]->isTouchedByBall(ball)) {
				this->bricks[i]->performEvent(event);
				break;
			}
		}
	}else if (event.user.code == CustomEventUtils::Code::BRICK_REMOVED) {
		Brick *brick = (Brick*) event.user.data1;
		int index = this->findIndex(brick);
		this->bricks.erase(bricks.begin() + index);
		delete brick;
	}
}

int Wall::findIndex(Brick *brick) {
	int result = -1;
	if (this->bricks.size() > 0){
		int size = (int) bricks.size();
		for (int i = 0; i < size; i++){
			if (bricks[i] == brick){
				result = i;
				break;
			}
		}
	}
	return result;
}

void Wall::init() {
	std::vector<Brick*> bricks;
	for (int y = 0; y < GlobalConstants::MAX_NUMBER_OF_BRICKS_ON_Y; y++) {
		for (int x = 0; x < GlobalConstants::MAX_NUMBER_OF_BRICKS_ON_X; x++) {
			Brick *brick = new Brick();
			brick->init();
			brick->getTextureWithPosition()->setX(x * brick->getTextureWithPosition()->getPosition().w);
			brick->getTextureWithPosition()->setY(y * brick->getTextureWithPosition()->getPosition().h);
			bricks.push_back(brick);
		}
	}
	setBricks(bricks);
	build();
}
