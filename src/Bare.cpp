#include <SDL2/SDL_mixer.h>
#include <cmath>
#include <iostream>
#include "../include/Bare.hpp"
#include "../include/InitUtils.hpp"
#include "../include/CustomEventUtils.hpp"
#include "../include/Ball.h"
#include "../include/GlobalConstants.h"

Bare::Bare() {
	this->sound = nullptr;
	this->textureWithPosition = nullptr;
	InitUtils::getInstance()->addTexture("resources/images/bare.png", TEXTURE_KEY);
}

Bare::~Bare() {
	delete (this->textureWithPosition);
}

void Bare::setTextureWithPosition(TextureWithPosition *textureWithPosition) {
	this->textureWithPosition = textureWithPosition;
}

TextureWithPosition* Bare::getTextureWithPosition() {
	return this->textureWithPosition;
}

void Bare::render() {
	SDL_RenderCopy(InitUtils::getInstance()->getRenderer(), this->textureWithPosition->getTexture(), NULL, &(this->textureWithPosition->getPosition()));
}

Mix_Chunk* Bare::getSound() {
	return sound;
}

void Bare::setSound(Mix_Chunk *sound) {
	this->sound = sound;
}

void Bare::init() {
	int widhtBare;
	int heightBare;
	SDL_QueryTexture(InitUtils::getInstance()->getMapTextures()[TEXTURE_KEY], nullptr, nullptr, &widhtBare, &heightBare);
	int posX = (GlobalConstants::BALL_ZONE_X + GlobalConstants::BALL_ZONE_WIDTH - widhtBare) / 2;
	int posY = GlobalConstants::BALL_ZONE_Y + GlobalConstants::BALL_ZONE_HEIGHT - heightBare;
	if (textureWithPosition == nullptr) {
		this->textureWithPosition = new TextureWithPosition(InitUtils::getInstance()->getMapTextures()[TEXTURE_KEY], posX, posY, widhtBare, heightBare);
	} else {
		this->textureWithPosition->setX(posX);
		this->textureWithPosition->setY(posY);
	}
	setSound(InitUtils::getInstance()->getMapSounds()[Bare::SOUND_KEY]);
}

void Bare::performEvent(SDL_Event &event) {
	switch (event.type) {
	case SDL_MOUSEMOTION:
		getTextureWithPosition()->setX(getTextureWithPosition()->getX() + event.motion.xrel);
		break;
	case SDL_USEREVENT:
		switch (event.user.code) {
			case CustomEventUtils::Code::BALL_MOVED:
				bounces((Ball*) event.user.data1);
				break;
			case CustomEventUtils::Code::BORDER_BOTTOM_TOUCHED:
				init();
				break;
			case CustomEventUtils::Code::SURPRISE_BRICK_FALLING:
				if (isSurpriseCaught((Brick *) event.user.data1)){
					CustomEventUtils::getInstance()->postEventSurpriseBrickCatch();
				}
			break;
		}
		break;
	}
}

bool Bare::isSurpriseCaught(Brick *brick) {
     SDL_Rect brickRect = brick->getTextureWithPosition()->getPosition();
     SDL_Rect bareRect = this->getTextureWithPosition()->getPosition();
     return (SDL_HasIntersection(&brickRect, &bareRect) == SDL_TRUE);
}

bool Bare::bounces(Ball *ball) {
	bool result = false;
    if (ball->getTextureWithPosition()->getX2() >= this->getTextureWithPosition()->getX()
            && ball->getTextureWithPosition()->getX() <= this->getTextureWithPosition()->getX2()
            && ball->getTextureWithPosition()->getY2() >= this->getTextureWithPosition()->getY()
            && ball->getTextureWithPosition()->getY() <= this->getTextureWithPosition()->getY2()) {
        if (!ball->isGlued()) {
            Mix_PlayChannel(-1, InitUtils::getInstance()->getMapSounds()[GlobalConstants::BARE_SOUND_KEY], 0);
            int centerBare = this->getTextureWithPosition()->getAbsCenterX();
            int centerBall = ball->getTextureWithPosition()->getAbsCenterX();
            int halfWidthBare = (this->getTextureWithPosition()->getPosition().w + ball->getTextureWithPosition()->getPosition().w) / 2;
            int deltaCenter = centerBall - centerBare;
            int segment = (deltaCenter * 5) / halfWidthBare;  // Divise la barre en 5 segments (-2 à 2)
            float speed = (float) ball->getSpeed();
            printf(" Segment %d \n",segment);
            switch (segment) {
                case -4: 
                    ball->setDirX(-speed * 0.73f);
                    ball->setDirY(-speed * 0.68f);
                    break;
                case -3:
                    ball->setDirX(-speed * 0.65f);
                    ball->setDirY(-speed * 0.75f);
                    break;
                case -2:
                    ball->setDirX(-speed * 0.5f);
                    ball->setDirY(-speed * 0.86f);
                    break;
                case -1:
                    ball->setDirX(-speed * 0.30f);
                    ball->setDirY(-speed * 0.95f);
                    break;
                case 0:
                    ball->setDirX(0);
                    ball->setDirY(-speed);
                    break;
                case 1:
                    ball->setDirX(speed * 0.30f);
                    ball->setDirY(-speed * 0.95f);
                    break;
                case 2:
                    ball->setDirX(speed * 0.5f);
                    ball->setDirY(-speed * 0.86f);
                    break;
                case 3:   
                    ball->setDirX(speed * 0.65f);
                    ball->setDirY(-speed * 0.75f);
                    break;
                case 4:  
                    ball->setDirX(speed * 0.73f);
                    ball->setDirY(-speed * 0.68f);
                    break;
                default:
                    ball->setDirX(0);
                    ball->setDirY(-speed);
                    break;
            }
            std::cout << "DEBUG Segment = " << segment << std::endl;
            std::cout << "DEBUG DirX = " << ball->getDirX() << ", DirY = " << ball->getDirY() << std::endl;
            result = true;
            CustomEventUtils::getInstance()->postEventBareTouched();
        } else {
            ball->setDirX(0);
            ball->setDirY(0);
        }
    }
    return result;
}
