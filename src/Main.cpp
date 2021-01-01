#include <iostream>
#include <map>
#include <vector>
#include <SDL2/SDL.h>

#include "../include/Background.hpp"
#include "../include/Ball.h"
#include "../include/Bare.h"
#include "../include/Brick.h"
#include "../include/CustomEventUtils.hpp"
#include "../include/InitUtils.hpp"
#include "../include/MicroModSDLPlayer.hpp"
#include "../include/ScoreSegments.hpp"
#include "../include/TextureWithPosition.h"
#include "../include/Title.hpp"
#include "../include/Wall.hpp"
#include "../include/GameState.hpp"
#ifdef __cplusplus
#include <cstdlib>
#else
#include <stdlib.h>
#endif

int main(int argc, char **argv) {
	int posRand = 0;
	SDL_Texture *baseTexture = InitUtils::getInstance()->getBaseTexture();
	SDL_SetRelativeMouseMode(SDL_TRUE);
	const Uint8 *keys = SDL_GetKeyboardState(NULL);
	std::map<int, bool> mapKbd;
	SDL_Event event;
	Bare *bare = new Bare();
	bare->init();
	Wall *wall = new Wall();
	wall->init();
	ScoreSegments *scoreSegments = new ScoreSegments();
	scoreSegments->init();
	Ball *ball = new Ball();
	ball->init(wall, bare);
	Background *background = new Background();
	background->init();
	Title *title = new Title();
	title->init();
	bool loop = true;
	//Test amiga mod
	InitUtils::getInstance()->addMod("worldofw.mod", "mod_001");
	std::vector<unsigned char> modFile = InitUtils::getInstance()->getMapMods()["mod_001"];
	MicroModSDLPlayer::getInstance()->initialise(&modFile[0]);
	SDL_PauseAudio(0);
	//End test
	while (loop) {
		SDL_SetRenderTarget(InitUtils::getInstance()->getRenderer(), baseTexture);
		SDL_RenderClear(InitUtils::getInstance()->getRenderer());
		ball->moveBall();
		while (SDL_PollEvent(&event)) {
			switch (event.type) {
			case SDL_KEYUP:
				mapKbd[event.key.keysym.scancode] = false;
				break;
			case SDL_KEYDOWN:
				keys = SDL_GetKeyboardState(NULL);
				break;
			case SDL_MOUSEMOTION:
				if (GameStates::getInstance()->isStarted()) {
					bare->getTextureWithPosition()->setX(bare->getTextureWithPosition()->getX() + event.motion.xrel);
				}
				break;
			case SDL_MOUSEBUTTONUP:
				std::cout << "Mouse up..." << std::endl;
				if (event.button.button == SDL_BUTTON_LEFT) {
					if (!GameStates::getInstance()->isStarted()) {
						GameStates::getInstance()->setCurrentLevel(1);
						GameStates::getInstance()->setPaused(false);
						GameStates::getInstance()->setRemainingLives(5);
						GameStates::getInstance()->setStarted(true);
						GameStates::getInstance()->setScore(0);
					}
				}
				break;
			case SDL_USEREVENT:
				if (event.user.code == CustomEventUtils::Code::SONG_STOP) {
					SDL_PauseAudio(1);
					MicroModSDLPlayer::getInstance()->initialise(&modFile[0]);
					SDL_PauseAudio(0);
				} else if (event.user.code == CustomEventUtils::Code::BRICK_TOUCHED) {
					Brick *brick = (Brick*) event.user.data1;
					GameStates::getInstance()->addScore(brick->getValue());
				} else if (event.user.code == CustomEventUtils::Code::BORDER_BOTTOM_TOUCHED) {
					GameStates::getInstance()->decreaseRemainingLive(1);
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
					std::cout << "BAL MOVED" << std::endl;
					wall->performEvent(event);
				}
				break;
			}
		}
		if (keys[SDL_SCANCODE_ESCAPE]) {
			loop = false;
		}

		if (!GameStates::getInstance()->isStarted()) {
			int halfBarSize = bare->getTextureWithPosition()->getOriginRect().w / 2;
			int ballPosX = ball->getTextureWithPosition()->getAbsCenterX();
			int barePosX = ballPosX - halfBarSize + posRand;
			bare->getTextureWithPosition()->setX(barePosX);
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