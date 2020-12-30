/*
 * ScoreSegments.h
 *
 *  Created on: 9 avr. 2016
 *      Author: Papa
 */

#ifndef SCORESEGMENTS_H_
#define SCORESEGMENTS_H_
#include <SDL2/SDL.h>

class ScoreSegments {
public:
	ScoreSegments();
	virtual ~ScoreSegments();
	void render();
	void render(int score);
	void setTexture(SDL_Texture* sdlTexture);
	SDL_Rect calculateSrcRect(int figure );
	void addScore(int value);
private:
	int posX;
	int posY;
	int numberOfDigit;
	int score;
	SDL_Texture* sdlTexture;
};

#endif /* SCORESEGMENTS_H_ */
