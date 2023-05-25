#include "../include/Background.hpp"
#include "../include/InitUtils.hpp"
#include "../include/UtilConstants.h"
#include <SDL2/SDL.h>

using namespace std;

Background::Background(string fileName) {
	SDL_Texture* texture = InitUtils::getInstance()->loadTexture(fileName);
	this->textureWithPosition = new TextureWithPosition(texture, GlobalConstants::BALL_ZONE_X, GlobalConstants::BALL_ZONE_Y, GlobalConstants::BALL_ZONE_WIDTH,
			GlobalConstants::BALL_ZONE_HEIGHT);

}

Background::~Background() {
	delete (textureWithPosition);
}

void Background::render() {
	SDL_RenderCopy(InitUtils::getInstance()->getRenderer(), textureWithPosition->getTexture(), nullptr, &textureWithPosition->getPosition());
}

