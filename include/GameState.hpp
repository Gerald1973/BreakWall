/*
 * GameState.hpp
 *
 *  Created on: Dec 30, 2020
 *      Author: private
 */

#ifndef INCLUDE_GAMESTATE_HPP_
#define INCLUDE_GAMESTATE_HPP_

#endif /* INCLUDE_GAMESTATE_HPP_ */
#include <iostream>

class GameStates {
public:
	static inline GameStates* getInstance() {
		if (!instance) {
			instance = new GameStates();
			instance->reset();
		}
		return instance;
	}

	static inline void destroy() {
		if (instance) {
			delete instance;
			instance = NULL;
		}
	}

	void addScore(int number);
	void increaseLevelBy(int number);
	int getCurrentLevel();
	void setCurrentLevel(int currentLevel);
	bool isPaused();
	void setPaused(bool paused);
	int getRemainingBricks();
	int getRemainingLives();
	void setRemainingLives(int remainingLives);
	bool isStarted();
	void setStarted(bool started);
	void decreaseRemainingLive(int number);
	void reset();
	int getScore();
	void setRemainingBricks(int remainingBricks);
	void setScore(int score);
	bool isFullScreen();
	void setFullScreen(bool fullScreen);


private:
	int remainingBricks;
	int remainingLives;
	int currentLevel;
	bool paused;
	int score;
	bool started;
	bool fullScreen;
	static inline GameStates *instance;
	GameStates();
	virtual ~GameStates();
};
