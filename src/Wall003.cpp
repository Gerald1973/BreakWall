// src/Wall003.cpp
#include "../include/Wall003.hpp"
#include <SDL2/SDL.h>
#include "../include/InitUtils.hpp"
#include "../include/GlobalConstants.h"

void Wall003::init()
{
	std::vector<Brick *> bricks;
	this->background = new Background("resources/images/super_heros_female.png");

	int brickHeight, brickWidth;
	SDL_QueryTexture(InitUtils::getInstance()->getMapTextures()[GlobalConstants::TEXTURE_KEY], nullptr, nullptr, &brickWidth, &brickHeight);

	const int GRID_WIDTH = 15;
	const int GRID_HEIGHT = 8;

	// Motif du cœur (1 = brique, 0 = vide)
	int heartPattern[GRID_HEIGHT][GRID_WIDTH] = {
		{0, 0, 0, 1, 1, 1, 0, 0, 1, 1, 1, 0, 0, 0, 0},
		{0, 0, 1, 0, 0, 0, 1, 1, 0, 0, 0, 1, 0, 0, 0},
		{0, 1, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1, 0, 0},
		{0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0},
		{0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0},
		{0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0},
		{0, 0, 0, 0, 1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0},
		{0, 0, 0, 0, 0, 1, 0, 0, 1, 0, 0, 0, 0, 0, 0}};

	// Placer les briques selon le motif
	for (int y = 0; y < GRID_HEIGHT; y++)
	{
		for (int x = 0; x < GRID_WIDTH; x++)
		{
			if (heartPattern[y][x] == 1)
			{
				Brick *brick = new Brick();
				brick->init();
				brick->getTextureWithPosition()->setX(x * brickWidth);
				brick->getTextureWithPosition()->setY(y * brickHeight);
				bricks.push_back(brick);
			}
		}
	}

	setBricks(bricks);
	initSong("Wall003");
}