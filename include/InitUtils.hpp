/*
 * InitUtils.hpp
 *
 *  Created on: Dec 30, 2020
 *      Author: private
 */

#ifndef INCLUDE_INITUTILS_HPP_
#define INCLUDE_INITUTILS_HPP_

#include <SDL2/SDL.h>

class InitUtils {
public:
	int initRenderer();

	static inline InitUtils* getInstance() {
		if (!instance) {
			instance = new InitUtils();
		}
		return instance;
	}

	SDL_Renderer* getRenderer();
	SDL_Window* getPWindow();

private:
	SDL_Renderer *renderer;
	SDL_Window *pWindow;
	inline static InitUtils *instance;
	InitUtils();
	virtual ~InitUtils();
};

#endif /* INCLUDE_INITUTILS_HPP_ */
