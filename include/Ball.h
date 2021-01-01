#ifndef BALL_H
#define BALL_H
#include <SDL2/SDL.h>
#include <string>

#include "Bare.h"
#include "TextureWithPosition.h"

class Ball {
public:
	static inline std::string TEXTURE_KEY = "ball";
	Ball();
	virtual ~Ball();
	void render();
	void moveBall();
	Bare* getBare();
	void setBare(Bare* bare);
	TextureWithPosition* getTextureWithPosition();
	void setTextureWithPosition(TextureWithPosition* textureWithPosition);
	float getSpeed();
	void setSpeed(float speed);
	bool bouncesOnBare(Bare* bare);
	bool bouncesOnScreen();
	void setDirX(float dirX);
	float getDirX();
	void setDirY(float dirY);
	float getDirY();
	void init(Bare *bare);
protected:
private:
	TextureWithPosition* textureWithPosition;
	float dirX;
	float dirY;
	float speed;
	Bare* bare;
	float coeffX;
	float coeffy;
	float posX;
	float posY;
};

#endif // BALL_H
