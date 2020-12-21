/*
 * Background.h
 *
 *  Created on: 9 avr. 2016
 *      Author: Papa
 */
#include <SDL2/SDL.h>
#include "TextureWithPosition.h"

class Background
{
public:
	Background();
	virtual ~Background();
	void render(SDL_Renderer *renderer);
	void render(SDL_Renderer *renderer, int score);
	TextureWithPosition *getTextureWithPosition();
	void setTextureWithPosition(TextureWithPosition *textureWithPosition);

private:
	TextureWithPosition *textureWithPosition;
};