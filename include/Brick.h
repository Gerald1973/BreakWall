#ifndef BRICK_H
#define BRICK_H
#include <iostream>
#include <SDL2/SDL.h>
#include <SDL2/SDL_mixer.h>
#include "TextureWithPosition.h"

class Brick {
public:
	static inline std::string TEXTURE_KEY = "brick";
	static inline std::string SOUND_KEY = "brick";
	Brick();
	virtual ~Brick();
	void setValue(int value);
	int getValue();
	bool isDestroyed();
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
