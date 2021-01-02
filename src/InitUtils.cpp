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

InitUtils::InitUtils() {
	renderer = NULL;
	pWindow = NULL;
	baseTexture = NULL;
	initRenderer();
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

std::map<std::string, Mix_Chunk*> InitUtils::getMapSounds() {
	return mapSounds;
}

std::map<std::string, SDL_Texture*> InitUtils::getMapTextures() {
	return mapTextures;
}

void InitUtils::addMod(std::string fileName, std::string key) {
	if (mapMods.find(key) != mapMods.end()) {
		std::cout << "WARNING: The following key " << key << " is already presents in the map mods." << std::endl;
	} else {
		std::vector<unsigned char> results = FileUtils::getInstance()->readFile(fileName);
		mapMods[key] = results;
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

std::map<std::string, std::vector<unsigned char>> InitUtils::getMapMods() {
	return mapMods;
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

