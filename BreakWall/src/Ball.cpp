#include "Ball.h"
#include <SDL2/SDL_mixer.h>

Ball::Ball() {
	this->dirX = 1;
	this->dirY = 1;
	this->speed = 4;
	this->coeffX = 1;
	this->coeffX = 0;
	this->wall = NULL;
	this->textureWithPosition = NULL;
	this->posX = 0;
	this->posY = 0;
	this->bare = NULL;
}

Ball::~Ball() {
	delete (textureWithPosition);
}

void Ball::render(SDL_Renderer* renderer) {
	SDL_RenderCopy(renderer, getTextureWithPosition()->getTexture(), NULL,
			&(getTextureWithPosition()->getPosition()));
}

void Ball::moveBall() {
	bouncesOnScreen();
	bouncesOnBare(getBare());
	//2° the wall
	std::vector<std::vector<Brick*> > bricks = getWall()->getBricks();
	bool brickTouched = false;
	for (unsigned int j = 0; j < bricks.size(); j++) {
		for (unsigned int i = 0; i < bricks[j].size(); i++) {
			if (bouncesOnBrick(bricks[j][i])) {
				brickTouched = true;
				break;
			}
		}
		if (brickTouched) {
			break;
		}
	}
	//4° Move
	posX = posX + dirX;
	posY = posY + dirY;
	getTextureWithPosition()->setX(posX);
	getTextureWithPosition()->setY(posY);

}

Bare* Ball::getBare() {
	return bare;
}

void Ball::setBare(Bare* bare) {
	this->bare = bare;
}

TextureWithPosition* Ball::getTextureWithPosition() {
	return this->textureWithPosition;
}

void Ball::setTextureWithPosition(TextureWithPosition* textureWithPosition) {
	this->textureWithPosition = textureWithPosition;
	posX = textureWithPosition->getX();
	posY = textureWithPosition->getY();
}

Wall* Ball::getWall() {
	return wall;
}

void Ball::setWall(Wall* wall) {
	this->wall = wall;
}

bool Ball::bouncesOnBrick(Brick* brick) {
	bool result = false;
	if (!brick->isDestroyed()
			&& getTextureWithPosition()->getX2()
					>= brick->getTextureWithPosition()->getX()
			&& getTextureWithPosition()->getX()
					<= brick->getTextureWithPosition()->getX2()
			&& getTextureWithPosition()->getY()
					<= brick->getTextureWithPosition()->getY2()
			&& getTextureWithPosition()->getY2()
					>= brick->getTextureWithPosition()->getY()) {
		brick->setDestroyed(true);
		Mix_PlayChannel( -1,brick->getSound(), 0 );
		if (getTextureWithPosition()->getAbsCenterX()
				< brick->getTextureWithPosition()->getX()) {
			dirX = -dirX;
			posX = brick->getTextureWithPosition()->getX()
					- getTextureWithPosition()->getPosition().w + dirX;
		} else if (getTextureWithPosition()->getAbsCenterX()
				> brick->getTextureWithPosition()->getX2()) {
			dirX = -dirX;
			posX = brick->getTextureWithPosition()->getX2() + dirX;
		}
		if (getTextureWithPosition()->getAbsCenterY()
				< brick->getTextureWithPosition()->getY()) {
			//std::cout << "haut " << "brick y2 = " << getTextureWithPosition()->getY2() << "bal posY = " << posY << "\n";
			dirY = -dirY;
			posY = brick->getTextureWithPosition()->getY()
					- getTextureWithPosition()->getPosition().h + dirY;
		} else if (getTextureWithPosition()->getAbsCenterY()
				> brick->getTextureWithPosition()->getY2()) {
			//std::cout << "bas " << "brick y2 = " << getTextureWithPosition()->getY2() << "bal posY = " << posY << "\n";
			dirY = -dirY;
			posY = brick->getTextureWithPosition()->getY2() + dirY;
		}
		result = true;

	}
	return result;
}

bool Ball::bouncesOnScreen() {
	bool result = false;
	if (posX <= UtilConstants::getInstance()->screenSize.x) {
		dirX = -dirX;
		posX = UtilConstants::getInstance()->screenSize.x;
		result = true;
	}
	if (posX + getTextureWithPosition()->getPosition().w
			>= UtilConstants::getInstance()->screenSize.x
					+ UtilConstants::getInstance()->screenSize.w) {
		dirX = -dirX;
		posX = posX - this->getTextureWithPosition()->getPosition().w;
		result = true;
	}
	if (posY <= UtilConstants::getInstance()->screenSize.y) {
		dirY = -dirY;
		posY = UtilConstants::getInstance()->screenSize.y;
		result = true;
	}
	if (posY + getTextureWithPosition()->getPosition().h
			>= UtilConstants::getInstance()->screenSize.y
					+ UtilConstants::getInstance()->screenSize.h) {
		dirY = -dirY;
		posY = posY - this->getTextureWithPosition()->getPosition().h;
		result = true;
	}
	return result;

}

bool Ball::bouncesOnBare(Bare* bare) {
	bool result = false;
	if (getTextureWithPosition()->getX2()
			>= bare->getTextureWithPosition()->getX()
			&& getTextureWithPosition()->getX()
					<= bare->getTextureWithPosition()->getX2()
			&& getTextureWithPosition()->getY()
					<= bare->getTextureWithPosition()->getY2()
			&& getTextureWithPosition()->getY2()
					>= bare->getTextureWithPosition()->getY()) {
		Mix_PlayChannel( -1,bare->getSound(), 0 );
		dirY = -dirY;
		//calcul coeffX
		float centerBare =
				(float) (bare->getTextureWithPosition()->getAbsCenterX());
		float deltaCenter = (float) (getTextureWithPosition()->getAbsCenterX())
				- centerBare;
		float halfWidht =
				(float) (bare->getTextureWithPosition()->getPosition().w / 2);
		coeffX = deltaCenter / halfWidht;
		dirX = coeffX;
		dirY = sqrt(speed * speed - dirX * dirX);
		if (getTextureWithPosition()->getAbsCenterX()
				< bare->getTextureWithPosition()->getX()) {
			dirX = -dirX;
			posX = bare->getTextureWithPosition()->getX()
					- getTextureWithPosition()->getPosition().w + dirX;
		} else if (getTextureWithPosition()->getAbsCenterX()
				> bare->getTextureWithPosition()->getX2()) {
			if (dirX < 0) {
				dirX = -dirX;
			}
			posX = bare->getTextureWithPosition()->getX2() + dirX;
		}
		if (getTextureWithPosition()->getAbsCenterY()
				< bare->getTextureWithPosition()->getY()) {
			//std::cout << "haut " << "brick y2 = " << getTextureWithPosition()->getY2() << "bal posY = " << posY << "\n";
			dirY = -dirY;
			posY = bare->getTextureWithPosition()->getY()
					- getTextureWithPosition()->getPosition().h + dirY;
		} else if (getTextureWithPosition()->getAbsCenterY()
				> bare->getTextureWithPosition()->getY2()) {
			//std::cout << "bas " << "brick y2 = " << getTextureWithPosition()->getY2() << "bal posY = " << posY << "\n";
			dirY = -dirY;
			posY = bare->getTextureWithPosition()->getY2() + dirY;
		}
		result = true;

	}
	return result;
}

