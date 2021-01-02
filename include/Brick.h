#ifndef BRICK_H
#define BRICK_H
#include <iostream>
#include <SDL2/SDL.h>
#include <SDL2/SDL_mixer.h>
#include "TextureWithPosition.h"
#include "../include/Ball.h"

class Brick {
public:
	Brick();
	virtual ~Brick();
	void setValue(int value);
	void performEvent(SDL_Event &event);
	int getValue();
	bool isDestroyed();
	bool isTouchedByBall(Ball* ball);
	void bounces(Ball* ball);
	void setDestroyed(bool destroyed);
	TextureWithPosition* getTextureWithPosition();
	void setTextureWithPosition(TextureWithPosition *textureWithPosition);
	void render();
	Mix_Chunk* getSound();
	void setSound(Mix_Chunk *sound);
	void playDestroy();
	void init();
protected:
private:
	int value;
	bool destroyed;
	TextureWithPosition *textureWithPosition;
	Mix_Chunk *sound;
	int deadDirection;

};

#endif // BRICK_H
