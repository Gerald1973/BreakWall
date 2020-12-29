/*
 * Title.hpp
 *
 *  Created on: Dec 29, 2020
 *      Author: private
 */

#ifndef INCLUDE_TITLE_HPP_
#define INCLUDE_TITLE_HPP_

#include <SDL2/SDL.h>

#include "../include/TextureWithPosition.h"

class Title {
public:
	Title();
	virtual ~Title();
	void render(SDL_Renderer *renderer);
	void render(SDL_Renderer *renderer, int score);
	TextureWithPosition* getTextureWithPosition();
	void setTextureWithPosition(TextureWithPosition *textureWithPosition);
private:
	TextureWithPosition *textureWithPosition;
};

#endif /* INCLUDE_TITLE_HPP_ */
