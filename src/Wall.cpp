#include "../include/Wall.hpp"

#include <SDL2/SDL.h>

#include "../include/Brick.h"
#include "../include/GlobalConstants.h"
#include "../include/UtilConstants.h"
Wall::Wall()
{
}

Wall::~Wall()
{
}

void Wall::build()
{
    for (unsigned int j = 0; j < this->bricks.size(); j++){
        for (unsigned int i = 0; i < this->bricks[j].size(); i ++){
                this->bricks[j][i]->getTextureWithPosition()->setX(i * this->bricks[j][i]->getTextureWithPosition()->getPosition().w);
                this->bricks[j][i]->getTextureWithPosition()->setY(j * this->bricks[j][i]->getTextureWithPosition()->getPosition().h);
        }
    }
}

void Wall::render(SDL_Renderer *renderer)
{
    for (unsigned int j = 0; j < bricks.size(); j++){
        for (unsigned int i = 0; i < bricks[j].size(); i ++){
            bricks[j][i]->render(renderer);
        }
    }
}

std::vector<std::vector<Brick* > > Wall::getBricks()
{
    return this->bricks;
}

void Wall::setBricks(std::vector<std::vector<Brick* > > bricks)
{
    this->bricks = bricks;
}