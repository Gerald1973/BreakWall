#ifndef BARE_H
#define BARE_H
#include <SDL2/SDL.h>
#include <SDL2/SDL_mixer.h>
//#include "Brick.h"

#include "TextureWithPosition.h"

class Ball;

class Bare {
public:
	static inline std::string TEXTURE_KEY = "bare";
	static inline std::string SOUND_KEY = "bare";
	Bare();
	virtual ~Bare();
	void init();
	void setTextureWithPosition(TextureWithPosition *textureWithPosition);
	TextureWithPosition* getTextureWithPosition();
	void performEvent(SDL_Event &event);
	void render();
	Mix_Chunk* getSound();
	void setSound(Mix_Chunk *sound);
	bool bounces(Ball *ball);
	//bool isSurpriseCaught(Brick* brick); 
protected:
private:
	TextureWithPosition *textureWithPosition;
	Mix_Chunk *sound;
};

#endif // BARE_H
