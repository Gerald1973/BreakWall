/*
 * InitUtils.cpp
 *
 *  Created on: Dec 30, 2020
 *      Author: private
 */

#include "../include/InitUtils.hpp"
#include <iostream>
#include <SDL2/SDL.h>
#include <SDL2/SDL_mixer.h>
#include "../include/GlobalConstants.h"

InitUtils::InitUtils() {
	renderer = NULL;
	pWindow = NULL;
	baseTexture = NULL;
}

int InitUtils::initRenderer() {
	// initialize SDL video
	if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO) < 0) {
		std::cout << "Unable to init SDL: " << SDL_GetError() << "\n";
		return 1;
	}
	if (Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 2048) < 0) {
		std::cout << "SDL_mixer could not initialize! SDL_mixer Error: " << Mix_GetError() << "\n";
		return 1;
	}
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 2);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 1);
	pWindow = SDL_CreateWindow("BreakWall", 0, 0, GlobalConstants::SCREEN_WIDTH, GlobalConstants::SCREEN_HEIGHT,
			SDL_WINDOW_OPENGL | SDL_WINDOW_OPENGL | SDL_WINDOW_RESIZABLE);
	if (pWindow == NULL) {
		std::cout << SDL_GetError() << "\n";
		return 1;
	}
	renderer = SDL_CreateRenderer(pWindow, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_TARGETTEXTURE | SDL_RENDERER_PRESENTVSYNC);
	baseTexture = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_ABGR8888, SDL_TEXTUREACCESS_TARGET, GlobalConstants::SCREEN_WIDTH,
				GlobalConstants::SCREEN_HEIGHT);
	return 0;
}

SDL_Renderer* InitUtils::getRenderer() {
	return renderer;
}

SDL_Window* InitUtils::getPWindow() {
	return pWindow;
}

SDL_Texture* InitUtils::getBaseTexture() {
	return baseTexture;
}

InitUtils::~InitUtils() {
	renderer = NULL;
}

