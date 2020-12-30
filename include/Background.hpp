/*
 * Background.h
 *
 *  Created on: 9 avr. 2016
 *      Author: Papa
 */
#include <SDL2/SDL.h>
#include <string>

#include "../include/TextureWithPosition.h"

class Background
{
public:
	static inline std::string TEXTURE_KEY="background001";
	Background();
	virtual ~Background();
	void render();
	TextureWithPosition *getTextureWithPosition();
	void setTextureWithPosition(TextureWithPosition *textureWithPosition);

private:
	TextureWithPosition *textureWithPosition;
};
