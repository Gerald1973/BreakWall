#ifndef BALL_H
#define BALL_H
#include <SDL2/SDL.h>
#include "TextureWithPosition.h"
#include "Bare.h"
#include "Wall.hpp"

class Ball {
public:
	Ball();
	virtual ~Ball();
	void render(SDL_Renderer* renderer);
	void moveBall();
	Bare* getBare();
	void setBare(Bare* bare);
	Wall* getWall();
	void setWall(Wall *wall);
	TextureWithPosition* getTextureWithPosition();
	void setTextureWithPosition(TextureWithPosition* textureWithPosition);
	float getSpeed();
	void setSpeed(float speed);
	bool isBrickTouched(Brick* brick);
	bool bouncesOnBare(Bare* bare);
	bool bouncesOnScreen();
	void bouncesOnBrick(Brick* brick);
protected:
private:
	TextureWithPosition* textureWithPosition;
	float dirX;
	float dirY;
	float speed;
	Bare* bare;
	Wall* wall;
	float coeffX;
	float coeffy;
	float posX;
	float posY;
};

#endif // BALL_H
