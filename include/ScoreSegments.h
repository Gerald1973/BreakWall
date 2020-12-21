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
	void render(SDL_Renderer* renderer);
	void render(SDL_Renderer* renderer,int score);
	void setTexture(SDL_Texture* sdlTexture);
	SDL_Rect caluclateSrcRect(int figure );
	void addScore(int value);
private:
	int posX;
	int posY;
	int numberOfDigit;
	int score;
	SDL_Texture* sdlTexture;
	static const int width=96;
	static const int height=128;
};

#endif /* SCORESEGMENTS_H_ */