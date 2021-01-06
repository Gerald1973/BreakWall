/*
 * InitUtils.cpp
 *
 *  Created on: Dec 30, 2020
 *      Author: private
 */

#include "../include/InitUtils.hpp"
#include <iostream>
#include <vector>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include "../include/GlobalConstants.h"
#include "../include/FileUtils.hpp"
#include <iostream>
#include "../include/WallRegistry.hpp"
#include "../include/Wall.hpp"
#include "../include/Wall002.hpp"

InitUtils::InitUtils() {
	renderer = NULL;
	pWindow = NULL;
	baseTexture = NULL;
	initRenderer();
	fillMixMusic();
	fillSoundEffect();
	fillTexture();
	registerWalls();
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
	addTexture("resources/images/brick_red_pop.png", GlobalConstants::TEXTURE_KEY);
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

std::map<std::string, Mix_Chunk*> InitUtils::getMapSounds() {
	return mapSounds;
}

std::map<std::string, SDL_Texture*> InitUtils::getMapTextures() {
	return mapTextures;
}

void InitUtils::addMixMusic(std::string fileName, std::string key) {
	if (mixMusics.find(key) != mixMusics.end()) {
		std::cout << "WARNING: The following key " << key << " is already presents in the map mixMusic." << std::endl;
	} else {
		Mix_Music *result = Mix_LoadMUS(fileName.c_str());
		if (result != nullptr) {
			mixMusics[key] = result;
		} else {
			std::cout << "ERROR:impossible to load the mod file " << fileName << " with the key " << key << std::endl;
		}
	}
}

void InitUtils::addSoundEffect(std::string fileName, std::string key) {
	if (mapSounds.find(key) != mapSounds.end()) {
		std::cout << "WARNING: The following key " << key << " is already present in the sound map." << std::endl;
	} else {
		Mix_Chunk *sound = Mix_LoadWAV(fileName.c_str());
		if (!sound) {
			std::cout << "Sound effect error : " << Mix_GetError() << std::endl;
		} else {
			mapSounds[key] = sound;
		}
	}
}

void InitUtils::addTexture(std::string fileName, std::string key) {
	if (mapTextures.find(key) != mapTextures.end()) {
		std::cout << "WARNING: The following key " << key << " is already present in the texture map." << std::endl;
	} else {
		SDL_Texture *result = NULL;
		result = IMG_LoadTexture(renderer, fileName.c_str());
		if (result == NULL) {
			std::cout << "Impossible to load : " << fileName << " error:" << SDL_GetError() << std::endl;
		} else {
			mapTextures[key] = result;
		}
	}
}

SDL_Texture* InitUtils::loadTexture(std::string fileName) {
	SDL_Texture *result = nullptr;
	result = IMG_LoadTexture(renderer, fileName.c_str());
	if (result == nullptr) {
		std::cout << "Impossible to load : " << fileName << " error:" << SDL_GetError() << std::endl;
	}
	return result;
}

bool InitUtils::toggleFullScreen() {
	bool result = true;
	Uint32 fullscreenFlag = SDL_WINDOW_FULLSCREEN_DESKTOP;
	bool isFullscreen = SDL_GetWindowFlags(this->pWindow) & fullscreenFlag;
	int tmp = SDL_SetWindowFullscreen(this->pWindow, isFullscreen ? 0 : fullscreenFlag);
	if (tmp < 0) {
		std::cout << "Error : fullscreen not possible" << std::endl;
		result = false;
	}
	return result;
}

void InitUtils::fillMixMusic() {
	this->addMixMusic("resources/mods/worldofw.mod", "Wall");
	this->addMixMusic("resources/mods/twinkle_by_fred_j.mod", "Wall002");
}

void InitUtils::fillSoundEffect() {
	this->addSoundEffect("resources/sound/scream.wav", GlobalConstants::BALL_DEAD_SOUND_KEY);
	this->addSoundEffect("resources/sound/bare.wav", GlobalConstants::BARE_SOUND_KEY);
	this->addSoundEffect("resources/sound/metal.wav", GlobalConstants::BRICK_SOUND_KEY);
}

void InitUtils::fillTexture() {
}

void InitUtils::registerWalls() {
	REGISTER_WALL(0, Wall::create);
	REGISTER_WALL(1, Wall002::create);
}

InitUtils::~InitUtils() {
	renderer = NULL;
	std::map<std::string, Mix_Chunk*>::iterator mapSoundsIterator;
	for (mapSoundsIterator = mapSounds.begin(); mapSoundsIterator != mapSounds.end(); mapSoundsIterator++) {
		std::cout << "Remove sound:" << mapSoundsIterator->first << "\n";
		Mix_FreeChunk(mapSoundsIterator->second);
	}
	std::map<std::string, SDL_Texture*>::iterator mapTexturesIterator;
	for (mapTexturesIterator = mapTextures.begin(); mapTexturesIterator != mapTextures.end(); mapTexturesIterator++) {
		std::cout << "Remove texture:" << mapTexturesIterator->first << "\n";
		SDL_DestroyTexture(mapTexturesIterator->second);
	}
	Mix_Quit();
	IMG_Quit();
	SDL_Quit();
	SDL_DestroyWindow(pWindow);
}

Mix_Music* InitUtils::getMixMusic(std::string key) {
	Mix_Music *result = nullptr;
	if (mixMusics.find(key) == mixMusics.end()) {
		std::cout << "DEBUG : The mod file " << key << " doesn't exist." << std::endl;
	} else {
		result = mixMusics[key];
	}
	return result;
}
