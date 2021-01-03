#ifndef BALL_H
#define BALL_H

#include <SDL2/SDL.h>
#include <string>

#include "Bare.hpp"
#include "TextureWithPosition.h"

class Ball {
public:
	static inline std::string TEXTURE_KEY = "ball";
	Ball();
	virtual ~Ball();
	void render();
	void moveBall();
	TextureWithPosition* getTextureWithPosition();
	void setTextureWithPosition(TextureWithPosition* textureWithPosition);
	float getSpeed();
	void setSpeed(float speed);
	bool bouncesOnScreen();
	void setDirX(float dirX);
	float getDirX();
	void setDirY(float dirY);
	float getDirY();
	void init();
	void setCoeffX(float coeffX);
	void setCoeffY(float coeffY);
	float getCoeffX();
	float getCoeffY();
protected:
private:
	TextureWithPosition* textureWithPosition;
	float dirX;
	float dirY;
	float speed;
	float coeffX;
	float coeffY;
	float posX;
	float posY;
};

#endif
