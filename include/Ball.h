#ifndef BALL_H
#define BALL_H

#include <SDL2/SDL.h>
#include <string>

#include "Bare.hpp"
#include "TextureWithPosition.h"

class Ball {
	public:
		Ball();
		virtual ~Ball();
		void render();
		void moveBall();
		TextureWithPosition* getTextureWithPosition();
		void setTextureWithPosition(TextureWithPosition* textureWithPosition);
		int getSpeed();
		void setSpeed(int speed);
		bool bouncesOnScreen();
		void setDirX(int dirX);
		int getDirX();
		void setDirY(int dirY);
		int getDirY();
		void init();
		void performEvent(SDL_Event &event);
		void setGlued(bool glued);
		bool isGlued();
	protected:
	private:
		TextureWithPosition* textureWithPosition;
		int dirX;
		int dirY;
		int speed;
		bool glued;
};
#endif
