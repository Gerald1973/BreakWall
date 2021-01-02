/*
 * InitUtils.hpp
 *
 *  Created on: Dec 30, 2020
 *      Author: private
 */

#ifndef INCLUDE_INITUTILS_HPP_
#define INCLUDE_INITUTILS_HPP_

#include <SDL2/SDL.h>
#include <SDL2/SDL_mixer.h>
#include <map>
#include <string>
#include <vector>

class InitUtils {
public:
	static inline InitUtils* getInstance() {
		if (!instance) {
			instance = new InitUtils();
		}
		return instance;
	}

	static inline void destroy() {
		if (instance) {
			delete instance;
			instance = NULL;
		}
	}

	SDL_Renderer* getRenderer();
	SDL_Texture* getBaseTexture();
	SDL_Window* getPWindow();
	std::map<std::string, Mix_Chunk*> getMapSounds();
	std::map<std::string, SDL_Texture*> getMapTextures();
	std::map<std::string, Mix_Music*> getMapMods();
	void addMixMusic(std::string fileName, std::string key);
	void addSoundEffect(std::string fileName, std::string key);
	void addTexture(std::string fileName, std::string key);
	bool toggleFullScreen();

private:
	std::map<std::string, Mix_Music*> mixMusics;
	std::map<std::string, Mix_Chunk*> mapSounds;
	std::map<std::string, SDL_Texture*> mapTextures;
	SDL_Renderer *renderer;
	SDL_Window *pWindow;
	SDL_Texture *baseTexture;
	inline static InitUtils *instance;
	int initRenderer();
	InitUtils();
	virtual ~InitUtils();
};

#endif /* INCLUDE_INITUTILS_HPP_ */
