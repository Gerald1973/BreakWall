#include "../include/Brick.h"
#include "SDL2/SDL.h"
#include "../include/InitUtils.hpp"
#include "../include/CustomEventUtils.hpp"
#include "../include/GlobalConstants.h"
#include <SDL2/SDL_mixer.h>

Brick::Brick() {
	this->value = 20;
	this->sound = NULL;
	this->destroyed = false;
	this->textureWithPosition = NULL;
	this->deadDirection = 1;
	this->animationDuration = 240;
}
void Brick::setValue(int value) {
	this->value = value;
}

int Brick::getValue() {
	return this->value;
}

TextureWithPosition* Brick::getTextureWithPosition() {
	return this->textureWithPosition;
}

void Brick::setTextureWithPosition(TextureWithPosition *textureWithPosition) {
	this->textureWithPosition = textureWithPosition;
}

Brick::~Brick() {
	std::cout << "DEBUG: Brick destruction." << std::endl;
	this->setTextureWithPosition(NULL);
	this->setDestroyed(true);
}

bool Brick::isDestroyed() {
	return this->destroyed;
}

void Brick::setDestroyed(bool destroyed) {
	this->destroyed = destroyed;
}

void Brick::render() {
	if (!isDestroyed()) {
        SDL_RenderCopy(InitUtils::getInstance()->getRenderer(), getTextureWithPosition()->getTexture(), NULL, &(getTextureWithPosition()->getPosition()));
    } else {
        playDestroy();
        if (this->animationFrame < this->animationDuration) {  // Rendre tant que l'animation n'est pas terminée
            SDL_RendererFlip sdlRendererFlip = (getTextureWithPosition()->getPosition().h == 0) ? SDL_FLIP_VERTICAL : SDL_FLIP_NONE;
            SDL_RenderCopyEx(InitUtils::getInstance()->getRenderer(),
                             getTextureWithPosition()->getTexture(),
                             NULL,
                             &(getTextureWithPosition()->getPosition()),
                             0,
                             nullptr,
                             sdlRendererFlip);
        }
    }
}

void Brick::playDestroy() {
    int y = getTextureWithPosition()->getY();
    int h = getTextureWithPosition()->getPosition().h;
	if (animationFrame <= animationDuration && y < GlobalConstants::BALL_ZONE_Y + GlobalConstants::BALL_ZONE_HEIGHT - h) {
		if (h == 0 || h == getTextureWithPosition()->getOriginRect().h) {
			deadDirection = -deadDirection;;
		}
		y = y + 2;
		h = h + deadDirection;
		getTextureWithPosition()->setY(y);
		getTextureWithPosition()->setH(h);
		animationFrame++;
	} else {
		CustomEventUtils::getInstance()->postEventBrickRemoved(this);
	}
}

Mix_Chunk* Brick::getSound() {
	return this->sound;
}

void Brick::setSound(Mix_Chunk *sound) {
	this->sound = sound;
}

bool Brick::isTouchedByBall(Ball *ball) {
	bool result = false;
	if (!isDestroyed()) {
		int x = ball->getTextureWithPosition()->getAbsCenterX();
		int y = ball->getTextureWithPosition()->getAbsCenterY();
		int halfBallSize = ball->getTextureWithPosition()->getPosition().w / 2;
		result = x >= this->getTextureWithPosition()->getX() - halfBallSize && x <= this->getTextureWithPosition()->getX2() + halfBallSize
				&& y >= this->getTextureWithPosition()->getY() - halfBallSize && y <= this->getTextureWithPosition()->getY2() + halfBallSize;
	}
	return result;
}

void Brick::bounces(Ball *ball) {
	Mix_PlayChannel(-1, InitUtils::getInstance()->getMapSounds()[GlobalConstants::BRICK_SOUND_KEY], 0);
    
    // Utiliser la largeur de la balle, pas celle de la brique
    int halfBallSize = ball->getTextureWithPosition()->getPosition().w / 2;
    int x = ball->getTextureWithPosition()->getAbsCenterX();
    int y = ball->getTextureWithPosition()->getAbsCenterY();

    // Définir les limites de la brique
    int brickLeft = getTextureWithPosition()->getX();
    int brickRight = getTextureWithPosition()->getX2();
    int brickTop = getTextureWithPosition()->getY();
    int brickBottom = getTextureWithPosition()->getY2();

    // Calculer les distances entre le centre de la balle et les faces de la brique
    int distTop = y - brickTop;
    int distBottom = brickBottom - y;
    int distLeft = x - brickLeft;
    int distRight = brickRight - x;

    // Déterminer la face la plus proche touchée
    int minDistVertical = distTop < distBottom ? distTop : distBottom;
    int minDistHorizontal = distLeft < distRight ? distLeft : distRight;

    // Si la collision est principalement verticale (haut ou bas)
    if (minDistVertical <= minDistHorizontal) {
        if (distTop < distBottom) { // Touche le haut
            ball->setDirY(-ball->getDirY());
            ball->getTextureWithPosition()->setY(brickTop - halfBallSize); // Corriger la position
        } else { // Touche le bas
            ball->setDirY(-ball->getDirY());
            ball->getTextureWithPosition()->setY(brickBottom + halfBallSize);
        }
    } else { // Collision principalement horizontale (gauche ou droite)
        if (distLeft < distRight) { // Touche la gauche
            ball->setDirX(-ball->getDirX());
            ball->getTextureWithPosition()->setX(brickLeft - halfBallSize);
        } else { // Touche la droite
            ball->setDirX(-ball->getDirX());
            ball->getTextureWithPosition()->setX(brickRight + halfBallSize);
        }
    }
}

void Brick::init() {
	SDL_Rect tmpRect;
	TextureWithPosition *textureWithPosition = new TextureWithPosition(InitUtils::getInstance()->getMapTextures()[GlobalConstants::TEXTURE_KEY], tmpRect);
	setTextureWithPosition(textureWithPosition);
}

void Brick::performEvent(SDL_Event &event) {
	if (event.user.code == CustomEventUtils::Code::BALL_MOVED) {
		Ball *ball = (Ball*) event.user.data1;
		if (this->isTouchedByBall(ball)) {
			this->bounces(ball);
			this->setDestroyed(true);
			CustomEventUtils::getInstance()->postEventBrickTouched(this);
		}
	}
}
