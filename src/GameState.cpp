/*
 * GameState.cpp
 *
 *  Created on: Dec 31, 2020
 *      Author: private
 */
#include "../include/GameState.hpp"
#include "../include/CustomEventUtils.hpp"

GameStates::GameStates() {
	reset();
}

GameStates::~GameStates() {
}

int GameStates::getCurrentLevel() {
	return currentLevel;
}

void GameStates::setCurrentLevel(int currentLevel) {
	this->currentLevel = currentLevel;
}

bool GameStates::isPaused() {
	return paused;
}

void GameStates::setPaused(bool paused) {
	this->paused = paused;
}

int GameStates::getRemainingLives() {
	return remainingLives;
}

void GameStates::setRemainingLives(int remainingLives) {
	this->remainingLives = remainingLives;
}

bool GameStates::isStarted() {
	return started;
}

void GameStates::setStarted(bool started) {
	this->started = started;
}

void GameStates::decreaseRemainingLive(int number) {
	this->remainingLives = this->remainingLives - number;
	if (this->remainingLives < 0) {
		CustomEventUtils::getInstance()->postEventLiveFinished();
	}
}

void GameStates::reset() {
	remainingLives = 5;
	currentLevel = 1;
	paused = false;
	started = false;
	score = 0;
}

int GameStates::getScore() {
	return score;
}

void GameStates::setScore(int score) {
	this->score = score;
}

void GameStates::addScore(int number) {
	score = score + number;
}