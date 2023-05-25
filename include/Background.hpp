#ifndef BACKGROUND_H
#define BACKGROUND_H
#include <SDL2/SDL.h>
#include <string>

#include "../include/TextureWithPosition.h"

class Background {
public:
	Background(std::string fileName);
	virtual ~Background();
	void render();
private:
	TextureWithPosition *textureWithPosition;
};
#endif
