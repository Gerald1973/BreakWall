#include "SDL2/SDL_audio.h"
#include "SDL2/SDL_error.h"
#include "SDL2/SDL_events.h"
#include "SDL2/SDL_keyboard.h"
#include "SDL2/SDL_main.h"
#include "SDL2/SDL_mouse.h"
#include "SDL2/SDL_rect.h"
#include "SDL2/SDL_render.h"
#include "SDL2/SDL_scancode.h"
#include "SDL2/SDL_stdinc.h"
#include "SDL2/SDL_video.h"
#include "TextureWithPosition.h"

#ifdef __cplusplus
#include <cstdlib>
#else
#include <stdlib.h>
#endif

#include <SDL2/SDL.h>
#include <iostream>
#include <map>
#include <string>
#include "../include/UtilConstants.h"
#include "../include/Brick.h"
#include "../include/Bare.h"
#include "../include/Wall.hpp"
#include "../include/Ball.h"
#include "../include/ScoreSegments.hpp"
#include <vector>
#include <SDL2/SDL_mixer.h>
#include <SDL2/SDL_image.h>
#include "../include/Background.hpp"
#include "../include/GlobalConstants.h"
#include "../include/FileUtils.hpp"
#include "../include/MicroModUtils.hpp"
#include "../include/MicroModSDLPlayer.hpp"
#include "../include/CustomEventUtils.hpp"
#include "../include/Title.hpp"
#include "../include/InitUtils.hpp"

Brick *wall[20][30];
int dirX = 1;
int dirY = 1;
bool success = true;

void displayErrorMessage() {
	std::cout << SDL_GetError() << "\n";
}

void initTextureMap() {
	InitUtils::getInstance()->addTexture("ball.png", "ball");
	InitUtils::getInstance()->addTexture("bare.png", "bare");
	InitUtils::getInstance()->addTexture("brickRed.bmp", "brickRed");
	InitUtils::getInstance()->addTexture("segments_48_64.png", "segments");
	InitUtils::getInstance()->addTexture("winter_01.jpg", "background001");
	InitUtils::getInstance()->addTexture("breakwall_title.png", "title");
}

void initSoundMap() {
	InitUtils::getInstance()->addSoundEffect("metal.wav", "brick");
	InitUtils::getInstance()->addSoundEffect("bare.wav", "brick");
}

Bare* initBare() {
	Bare *result = new Bare();
	SDL_Rect position;
	TextureWithPosition *textureWithPosition = new TextureWithPosition(InitUtils::getInstance()->getMapTextures()["bare"], position);
	textureWithPosition->setX((UtilConstants::getInstance()->gameZone.w - textureWithPosition->getPosition().w) / 2);
	textureWithPosition->setY(UtilConstants::getInstance()->gameZone.h - textureWithPosition->getPosition().h);
	result->setTextureWithPosition(textureWithPosition);
	result->setSound(InitUtils::getInstance()->getMapSounds()["bare"]);
	return result;
}

Wall* initWall() {
	Wall *result = new Wall();
	std::vector<std::vector<Brick*>> lines;
	for (int y = 0; y < GlobalConstants::MAX_NUMBER_OF_BRICKS_ON_Y; y++) {
		std::vector<Brick*> columns;
		for (int x = 0; x < GlobalConstants::MAX_NUMBER_OF_BRICKS_ON_X; x++) {
			Brick *brick = new Brick();
			brick->setSound(InitUtils::getInstance()->getMapSounds()["brick"]);
			SDL_Rect tmpRect;
			TextureWithPosition *textureWithPosition = new TextureWithPosition(InitUtils::getInstance()->getMapTextures()["brickRed"], tmpRect);
			brick->setTextureWithPosition(textureWithPosition);
			columns.push_back(brick);
		}
		lines.push_back(columns);
	}
	result->setBricks(lines);
	return result;
}

Ball* initBall(Wall *wall, Bare *bare) {
	SDL_Rect rect;
	rect.x = UtilConstants::getInstance()->gameZone.w / 2 + UtilConstants::getInstance()->gameZone.x;
	rect.y = (GlobalConstants::MAX_NUMBER_OF_BRICKS_ON_Y + 1) * GlobalConstants::BRICK_HEIGHT + UtilConstants::getInstance()->gameZone.y;
	TextureWithPosition *textureWithPosition = new TextureWithPosition(InitUtils::getInstance()->getMapTextures()["ball"], rect);
	Ball *ball = new Ball();
	ball->setTextureWithPosition(textureWithPosition);
	ball->setBare(bare);
	ball->setWall(wall);
	return ball;
}

Background* initBackground() {
	Background *result = new Background();
	TextureWithPosition *textureWithPosition = new TextureWithPosition(InitUtils::getInstance()->getMapTextures()["background001"],
			UtilConstants::getInstance()->gameZone.x, UtilConstants::getInstance()->gameZone.y, UtilConstants::getInstance()->gameZone.w,
			UtilConstants::getInstance()->gameZone.h);
	result->setTextureWithPosition(textureWithPosition);
	return result;
}

Title* initTitle() {
	Title *result = new Title();
	TextureWithPosition *textureWithPosition = new TextureWithPosition(InitUtils::getInstance()->getMapTextures()["title"], 0, 0, GlobalConstants::SCREEN_WIDTH,
			GlobalConstants::SCREEN_HEIGHT);
	result->setTextureWithPosition(textureWithPosition);
	return result;
}

ScoreSegments* initScoreSegments() {
	ScoreSegments *result = new ScoreSegments();
	result->setTexture(InitUtils::getInstance()->getMapTextures()["segments"]);
	return result;
}


int main(int argc, char **argv) {
	SDL_Window *pWindow = InitUtils::getInstance()->getPWindow();
	SDL_Texture *baseTexture = InitUtils::getInstance()->getBaseTexture();
	SDL_SetRelativeMouseMode(SDL_TRUE);
	const Uint8 *keys = SDL_GetKeyboardState(NULL);
	std::map<int, bool> mapKbd;
	SDL_Event event;
	initSoundMap();
	initTextureMap();
	Bare *bare = initBare();
	Wall *wall = initWall();
	wall->build();
	ScoreSegments *scoreSegments = initScoreSegments();
	Ball *ball = initBall(wall, bare);
	Background *background = initBackground();
	Title *title = initTitle();
	bool loop = true;
	//Test amiga mod
	InitUtils::getInstance()->addMod("worldofw.mod","mod_001");
	std::vector<unsigned char> mf =  InitUtils::getInstance()->getMapMods()["mod_001"];
	MicroModSDLPlayer::getInstance()->initialise(&mf[0]);
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
				bare->getTextureWithPosition()->setX(bare->getTextureWithPosition()->getX() + event.motion.xrel);
				break;
			case SDL_USEREVENT:
				if (event.user.code == CustomEventUtils::Code::SONG_STOP) {
					SDL_PauseAudio(1);
					MicroModSDLPlayer::getInstance()->initialise(&mf[0]);
					SDL_PauseAudio(0);
				} else if (event.user.code == CustomEventUtils::Code::BRICK_TOUCHED) {
					Brick *brick = (Brick*) event.user.data1;
					scoreSegments->addScore(brick->getValue());
				} else if (event.user.code == CustomEventUtils::Code::BORDER_BOTTOM_TOUCHED) {
					std::cout << "Todo one live less." << std::endl;
				}
				break;
			}
		}
		if (keys[SDL_SCANCODE_ESCAPE]) {
			loop = false;
		}
		background->render();
		wall->render();
		bare->render();
		ball->render();
		title->render();
		scoreSegments->render();
		SDL_SetRenderTarget(InitUtils::getInstance()->getRenderer(), NULL);
		SDL_RenderCopy(InitUtils::getInstance()->getRenderer(), baseTexture, NULL, NULL);
		SDL_RenderPresent(InitUtils::getInstance()->getRenderer());
	}
	SDL_DestroyWindow(pWindow);
	InitUtils::getInstance()->destroy();
	return 0;
}
