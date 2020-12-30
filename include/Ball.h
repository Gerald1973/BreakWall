#ifndef BALL_H
#define BALL_H
#include <SDL2/SDL.h>
#include <string>

#include "Bare.h"
#include "TextureWithPosition.h"
#include "Wall.hpp"

class Ball {
public:
	static inline std::string TEXTURE_KEY = "ball";
	Ball();
	virtual ~Ball();
	void render();
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
