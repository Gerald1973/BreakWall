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
	for (unsigned int j = 0; j < this->bricks.size(); j++) {
		for (unsigned int i = 0; i < this->bricks[j].size(); i++) {
			this->bricks[j][i]->getTextureWithPosition()->setX(i * this->bricks[j][i]->getTextureWithPosition()->getPosition().w);
			this->bricks[j][i]->getTextureWithPosition()->setY(j * this->bricks[j][i]->getTextureWithPosition()->getPosition().h);
		}
	}
}

void Wall::render() {
	for (unsigned int j = 0; j < bricks.size(); j++) {
		for (unsigned int i = 0; i < bricks[j].size(); i++) {
			bricks[j][i]->render();
		}
	}
}

std::vector<std::vector<Brick*> > Wall::getBricks() {
	return this->bricks;
}

void Wall::setBricks(std::vector<std::vector<Brick*> > bricks) {
	this->bricks = bricks;
}

void Wall::performEvent(SDL_Event &event) {
	if (event.user.code == CustomEventUtils::Code::BALL_MOVED) {
		std::cout << "Distribute the event to the bricks" << std::endl;
		for (unsigned int j = 0; j < this->bricks.size(); j++) {
			for (unsigned int i = 0; i < this->bricks[j].size(); i++) {
				this->bricks[j][i]->performEvent(event);
			}
		}
	}
}

void Wall::init() {
	std::vector<std::vector<Brick*>> lines;
	for (int y = 0; y < GlobalConstants::MAX_NUMBER_OF_BRICKS_ON_Y; y++) {
		std::vector<Brick*> columns;
		for (int x = 0; x < GlobalConstants::MAX_NUMBER_OF_BRICKS_ON_X; x++) {
			Brick *brick = new Brick();
			brick->init();
			columns.push_back(brick);
		}
		lines.push_back(columns);
	}
	setBricks(lines);
	build();
}
