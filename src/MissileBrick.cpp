#include "../include/MissileBrick.hpp"
#include "../include/InitUtils.hpp"
#include "../include/CustomEventUtils.hpp"
#include "../include/GlobalConstants.h"
#include "../include/Wall.hpp"
#include <cmath>
#include <iostream>

// Définir la taille du missile
const float MissileBrick::MISSILE_SIZE = 32.0f; // 16x16 pixels

MissileBrick::MissileBrick() : Brick()
{
	isMissileActive = false;
	missileDirX = 0.0f;
	missileDirY = 0.0f;
	missileSpeed = 10.0f; // 10 pixels par frame
}

MissileBrick::~MissileBrick()
{
	std::cout << "DEBUG: MissileBrick destruction." << std::endl;
}

void MissileBrick::init()
{
	SDL_Rect tmpRect;
	TextureWithPosition *textureWithPosition = new TextureWithPosition(
		InitUtils::getInstance()->getMapTextures()[GlobalConstants::MISSILE_BRICK_TEXTURE_KEY], tmpRect);
	setTextureWithPosition(textureWithPosition);
	isMissileActive = false;
}

void MissileBrick::performEvent(SDL_Event &event)
{
	if (event.user.code == CustomEventUtils::Code::BALL_MOVED && !isDestroyed() && !isMissileActive)
	{
		Ball *ball = (Ball *)event.user.data1;
		if (this->isTouchedByBall(ball))
		{
			getTextureWithPosition()->setTexture(InitUtils::getInstance()->getMapTextures()[GlobalConstants::MISSILE_TEXTURE_KEY]);
			launchMissile(ball);
			this->bounces(ball);
			this->setDestroyed(true);
			CustomEventUtils::getInstance()->postEventBrickTouched(this);
		}
	}
	else if (event.user.code == CustomEventUtils::Code::MISSILE_MOVED && !isDestroyed() && !isMissileActive)
	{
		Brick *missileBrick = (Brick *)event.user.data1;
		if (this->isTouchedByMissile(missileBrick))
		{
			this->setDestroyed(true);
			CustomEventUtils::getInstance()->postEventBrickTouched(this);
		}
	}
}

void MissileBrick::render()
{
	if (!isDestroyed() && !isMissileActive)
	{
		Brick::render(); // Rendre la brique normalement
		return;
	}

	if (isMissileActive)
	{
		updateMissile();

		// Rendre le missile (même texture, mais plus petite)
		SDL_Rect missileRect = getTextureWithPosition()->getPosition();
		missileRect.w = static_cast<int>(MISSILE_SIZE);
		missileRect.h = static_cast<int>(MISSILE_SIZE);
		SDL_RenderCopy(InitUtils::getInstance()->getRenderer(), getTextureWithPosition()->getTexture(),
					   nullptr, &missileRect);

		// Vérifier si le missile est hors des limites
		int x = getTextureWithPosition()->getX();
		int y = getTextureWithPosition()->getY();
		if (x <= 0 || x + missileRect.w >= GlobalConstants::BALL_ZONE_WIDTH ||
			y <= 0 || y + missileRect.h >= GlobalConstants::BALL_ZONE_HEIGHT)
		{
			isMissileActive = false;
			CustomEventUtils::getInstance()->postEventBrickRemoved(this);
		}
	}
}

void MissileBrick::launchMissile(Ball *ball)
{
	std::cout << "LAUNCH MISILE" << std::endl;
	isMissileActive = true;
	// Adopter la direction de la balle
	float speed = sqrt(ball->getDirX() * ball->getDirX() + ball->getDirY() * ball->getDirY());
	if (speed > 0)
	{
		missileDirX = (ball->getDirX() / speed) / 2;
		missileDirY = (ball->getDirY() / speed) / 2;
	}
	else
	{
		missileDirX = 0.0f;
		missileDirY = -1.0f; // Par défaut, vers le haut
	}
	// Centrer le missile
	int missileW = static_cast<int>(MISSILE_SIZE);
	int missileH = static_cast<int>(MISSILE_SIZE);
	getTextureWithPosition()->setW(missileW);
	getTextureWithPosition()->setH(missileH);
	getTextureWithPosition()->setX(getTextureWithPosition()->getAbsCenterX() - missileW / 2);
	getTextureWithPosition()->setY(getTextureWithPosition()->getAbsCenterY() - missileH / 2);
}

void MissileBrick::updateMissile()
{
	float newX = getTextureWithPosition()->getX() + missileDirX * missileSpeed;
	float newY = getTextureWithPosition()->getY() + missileDirY * missileSpeed;
	getTextureWithPosition()->setX(static_cast<int>(newX));
	getTextureWithPosition()->setY(static_cast<int>(newY));
	CustomEventUtils::getInstance()->postEventMissileMoved(this);
}