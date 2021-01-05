#include <iostream>
#include <map>
#include <vector>
#include <SDL2/SDL.h>

#include "../include/Background.hpp"
#include "../include/Ball.h"
#include "../include/Brick.h"
#include "../include/CustomEventUtils.hpp"
#include "../include/InitUtils.hpp"
#include "../include/ScoreSegments.hpp"
#include "../include/TextureWithPosition.h"
#include "../include/Title.hpp"
#include "../include/Wall.hpp"
#include "../include/GameState.hpp"
#include "../include/Bare.hpp"
#include "../include/WallRegistry.hpp"
#ifdef __cplusplus
#include <cstdlib>
#else
#include <stdlib.h>
#endif


void initBareAndBall(Bare* bare, Ball* ball){
	ball->init();
	bare->init();
	int ballPosX = bare->getTextureWithPosition()->getAbsCenterX() - ball->getTextureWithPosition()->getPosition().w / 2;
	int ballPosY = bare->getTextureWithPosition()->getY() - ball->getTextureWithPosition()->getPosition().h;
	std::cout << "DEBUG: ball Y = " << ballPosY << std::endl;
	ball->getTextureWithPosition()->setX(ballPosX);
	ball->getTextureWithPosition()->setY(ballPosY);
	ball->setGlued(true);
}

int main(int argc, char **argv) {
	int posRand = 0;
	SDL_Texture *baseTexture = InitUtils::getInstance()->getBaseTexture();
	SDL_SetRelativeMouseMode(SDL_TRUE);
	SDL_Event event;
	Bare *bare = new Bare();
	bare->init();
	Wall* wall = WallRegistry::create(GameStates::getInstance()->getCurrentLevel());
	wall->init();
	ScoreSegments *scoreSegments = new ScoreSegments();
	scoreSegments->init();
	Ball *ball = new Ball();
	ball->init();

	Background *background = new Background();
	background->init();
	Title *title = new Title();
	title->init();
	bool loop = true;
	while (loop) {
		SDL_SetRenderTarget(InitUtils::getInstance()->getRenderer(), baseTexture);
		SDL_RenderClear(InitUtils::getInstance()->getRenderer());
		ball->moveBall();
		while (SDL_PollEvent(&event)) {
			switch (event.type) {
			case SDL_KEYDOWN:
				switch (event.key.keysym.sym) {
				case SDLK_F11:
					GameStates::getInstance()->setFullScreen(InitUtils::getInstance()->toggleFullScreen());
					break;
				case SDLK_ESCAPE:
					loop = false;
					break;
				}
				break;
			case SDL_MOUSEMOTION:
				if (GameStates::getInstance()->isStarted()) {
					ball->performEvent(event);
					bare->performEvent(event);
				}
				break;
			case SDL_MOUSEBUTTONUP:
				std::cout << "Mouse up..." << std::endl;
				if (event.button.button == SDL_BUTTON_LEFT) {
					if (GameStates::getInstance()->isStarted()) {
						ball->performEvent(event);
						bare->performEvent(event);
					} else {
						GameStates::getInstance()->setCurrentLevel(1);
						GameStates::getInstance()->setPaused(false);
						GameStates::getInstance()->setRemainingLives(5);
						GameStates::getInstance()->setScore(0);
						wall = WallRegistry::create(GameStates::getInstance()->getCurrentLevel());
						wall->init();
						initBareAndBall(bare, ball);
						GameStates::getInstance()->setStarted(true);
					}
				}
				break;
			case SDL_USEREVENT:
				if (event.user.code == CustomEventUtils::Code::BRICK_TOUCHED) {
					Brick *brick = (Brick*) event.user.data1;
					GameStates::getInstance()->addScore(brick->getValue());
				} else if (event.user.code == CustomEventUtils::Code::BORDER_BOTTOM_TOUCHED) {
					GameStates::getInstance()->decreaseRemainingLive(1);
					initBareAndBall(bare, ball);
				} else if (event.user.code == CustomEventUtils::Code::LIVE_FINISHED) {
					GameStates::getInstance()->setCurrentLevel(1);
					GameStates::getInstance()->setPaused(false);
					GameStates::getInstance()->setRemainingLives(5);
					GameStates::getInstance()->setStarted(false);
				} else if (event.user.code == CustomEventUtils::Code::BARE_TOUCHED) {
					if (!GameStates::getInstance()->isStarted()) {
						int halfBarSize = bare->getTextureWithPosition()->getOriginRect().w / 2;
						posRand = rand() % (halfBarSize - 1);
						int posOrNeg = rand() % 2;
						if (posOrNeg == 0) {
							posRand = -posRand;
						}
					}
				} else if (event.user.code == CustomEventUtils::Code::BALL_MOVED) {
					wall->performEvent(event);
					bare->performEvent(event);
				} else if (event.user.code == CustomEventUtils::Code::BRICK_REMOVED) {
					wall->performEvent(event);
				} else if (event.user.code == CustomEventUtils::Code::BRICK_REMAINING) {
					std::cout << "DEBUG: Remaining bricks : " << wall->getBricks().size() << std::endl;
					GameStates::getInstance()->setRemainingBricks(wall->getBricks().size());
				}
				break;
			}
		}
		if (!GameStates::getInstance()->isStarted()) {
			ball->setGlued(false);
			int halfBarSize = bare->getTextureWithPosition()->getOriginRect().w / 2;
			int ballPosX = ball->getTextureWithPosition()->getAbsCenterX();
			int barePosX = ballPosX - halfBarSize + posRand;
			bare->getTextureWithPosition()->setX(barePosX);
		}

		if (GameStates::getInstance()->getRemainingBricks() == 0) {
			GameStates::getInstance()->increaseLevelBy(1);
			delete(wall);
			wall=WallRegistry::create(GameStates::getInstance()->getCurrentLevel() % 2);
			wall->init();
			initBareAndBall(bare, ball);
			GameStates::getInstance()->setRemainingBricks(wall->getBricks().size());
			if (!GameStates::getInstance()->isStarted()){
				ball->setGlued(false);
			}
		}

		background->render();
		wall->render();
		bare->render();
		ball->render();
		if (!GameStates::getInstance()->isStarted()) {
			title->render();
		}
		scoreSegments->render(GameStates::getInstance()->getScore());
		SDL_SetRenderTarget(InitUtils::getInstance()->getRenderer(), NULL);
		SDL_RenderCopy(InitUtils::getInstance()->getRenderer(), baseTexture, NULL, NULL);
		SDL_RenderPresent(InitUtils::getInstance()->getRenderer());
	}
	InitUtils::getInstance()->destroy();
	return 0;
}
