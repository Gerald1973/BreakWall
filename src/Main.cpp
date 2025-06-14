#include <SDL2/SDL.h>
#include <iostream>
#include <map>
#include <vector>

#include "../include/Background.hpp"
#include "../include/Ball.h"
#include "../include/Bare.hpp"
#include "../include/Brick.h"
#include "../include/CustomEventUtils.hpp"
#include "../include/GameState.hpp"
#include "../include/InitUtils.hpp"
#include "../include/ScoreSegments.hpp"
#include "../include/TextureWithPosition.h"
#include "../include/Title.hpp"
// #include "../include/TransitionManager.hpp"
#include "../include/Wall.hpp"
#include "../include/WallRegistry.hpp"
#ifdef __cplusplus
#include <cstdlib>
#else
#include <stdlib.h>
#endif

void initBareAndBall(Bare *bare, Ball *ball) {
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
  int targetBarePosX = 0;
  int frameCounter = 0;
  const int TARGET_UPDATE_INTERVAL = 15;
  const float LERP_FACTOR = 0.1f;
  SDL_Texture *baseTexture = InitUtils::getInstance()->getBaseTexture();
  SDL_SetRelativeMouseMode(SDL_TRUE);
  //SDL_SetRelativeMouseMode(SDL_FALSE);
  SDL_ShowCursor(SDL_ENABLE);
  SDL_Event event;
  Bare *bare = new Bare();
  bare->init();
  Wall *wall = WallRegistry::create(GameStates::getInstance()->getCurrentLevel());
  wall->init();
  GameStates::getInstance()->setRemainingBricks(wall->getBricks().size());
  ScoreSegments *scoreSegments = new ScoreSegments();
  scoreSegments->init();
  Ball *ball = new Ball();
  ball->init();
  Title *title = new Title();
  title->init();

  const int FPS = 60;
  const int frameDelay = 1000 / FPS;
  Uint32 frameStart;
  int frameTime;
  bool loop = true;

  // TransitionManager transitionManager(InitUtils::getInstance()->getRenderer(), GlobalConstants::SCREEN_WIDTH, GlobalConstants::SCREEN_HEIGHT);

  while (loop) {
    frameStart = SDL_GetTicks();
    SDL_SetRenderTarget(InitUtils::getInstance()->getRenderer(), baseTexture);
    SDL_RenderClear(InitUtils::getInstance()->getRenderer());

    if (!GameStates::getInstance()->isTransitioning()) {
      ball->moveBall();
    }

    while (SDL_PollEvent(&event)) {
      switch (event.type) {
      case SDL_QUIT:
        loop = false;
        break;
      case SDL_KEYDOWN:
        switch (event.key.keysym.sym) {
        case SDLK_F11:
          GameStates::getInstance()->setFullScreen(InitUtils::getInstance()->toggleFullScreen());
          break;
        case SDLK_ESCAPE:
          loop = false;
          break;
        case SDLK_F12:
          GameStates::getInstance()->setRemainingBricks(0);
          break;
        }
        break;
      case SDL_MOUSEMOTION:
        if (GameStates::getInstance()->isStarted() && !GameStates::getInstance()->isTransitioning()) {
          ball->performEvent(event);
          bare->performEvent(event);
        }
        break;
      case SDL_MOUSEBUTTONUP:
        std::cout << "Mouse up..." << std::endl;
        if (event.button.button == SDL_BUTTON_LEFT) {
          if (!GameStates::getInstance()->isTransitioning()) {
            if (GameStates::getInstance()->isStarted()) {
              ball->performEvent(event);
              bare->performEvent(event);
            } else {
              GameStates::getInstance()->setCurrentLevel(0);
              GameStates::getInstance()->setPaused(false);
              GameStates::getInstance()->setRemainingLives(5);
              GameStates::getInstance()->setScore(0);
              delete wall;
              wall = WallRegistry::create(GameStates::getInstance()->getCurrentLevel());
              if (!wall) {
                std::cerr << "Erreur : Impossible de créer le mur" << std::endl;
                loop = false;
                break;
              }
              wall->init();
              initBareAndBall(bare, ball);
              GameStates::getInstance()->setStarted(true);
            }
          }
        }
        break;
      case SDL_USEREVENT:
        if (event.user.code == CustomEventUtils::Code::BRICK_TOUCHED) {
          Brick *brick = (Brick *)event.user.data1;
          GameStates::getInstance()->addScore(brick->getValue());
        } else if (event.user.code == CustomEventUtils::Code::BORDER_BOTTOM_TOUCHED) {
          GameStates::getInstance()->decreaseRemainingLive(1);
          initBareAndBall(bare, ball);
        } else if (event.user.code == CustomEventUtils::Code::LIVE_FINISHED) {
          GameStates::getInstance()->setCurrentLevel(0);
          GameStates::getInstance()->setPaused(false);
          GameStates::getInstance()->setRemainingLives(5);
          GameStates::getInstance()->setStarted(false);
        } else if (event.user.code == CustomEventUtils::Code::BALL_MOVED) {
          wall->performEvent(event);
          bare->performEvent(event);
        } else if (event.user.code == CustomEventUtils::Code::MISSILE_MOVED) {
          wall->performEvent(event);
        } else if (event.user.code == CustomEventUtils::Code::BRICK_REMOVED) {
          wall->performEvent(event);
        } else if (event.user.code == CustomEventUtils::Code::BRICK_REMAINING) {
          std::cout << "DEBUG: Remaining bricks : " << wall->getBricks().size() << std::endl;
          GameStates::getInstance()->setRemainingBricks(wall->getBricks().size());
        } else if (event.user.code == CustomEventUtils::Code::SURPRISE_BRICK_FALLING) {
          bare->performEvent(event);
        } else if (event.user.code == CustomEventUtils::Code::SURPRISE_BRICK_CATCH) {
          wall->performEvent(event);
        }
        break;
      }
    }

    if (!GameStates::getInstance()->isStarted()) {
      ball->setGlued(false);
      if (frameCounter % TARGET_UPDATE_INTERVAL == 0) {
        frameCounter = 0;
        int halfBarSize = bare->getTextureWithPosition()->getOriginRect().w / 2;
        posRand = rand() % (halfBarSize - 1);
        int posOrNeg = rand() % 2;
        if (posOrNeg == 0)
          posRand = -posRand;
        int ballPosX = ball->getTextureWithPosition()->getAbsCenterX();
        targetBarePosX = ballPosX - halfBarSize + posRand; // Nouvelle position cible
      }
      int currentBarePosX = bare->getTextureWithPosition()->getX();
      int newBarePosX = currentBarePosX + static_cast<int>((targetBarePosX - currentBarePosX) * LERP_FACTOR);
      bare->getTextureWithPosition()->setX(newBarePosX);
      frameCounter++;
    }

    if (GameStates::getInstance()->getRemainingBricks() == 0) {
      GameStates::getInstance()->setTransitioning(false);
      GameStates::getInstance()->increaseLevelBy(1);
      delete wall;
      wall = WallRegistry::create(GameStates::getInstance()->getCurrentLevel() % 3);
      if (!wall) {
        std::cerr << "Erreur : Impossible de créer le mur" << std::endl;
        loop = false;
        break;
      }
      wall->init();
      initBareAndBall(bare, ball);
      GameStates::getInstance()->setRemainingBricks(wall->getBricks().size());
      if (!GameStates::getInstance()->isStarted()) {
        ball->setGlued(false);
      }
    }

    wall->render();
    bare->render();
    ball->render();
    scoreSegments->render(GameStates::getInstance()->getScore());
    if (!GameStates::getInstance()->isStarted()) {
      title->render();
    }

    SDL_SetRenderTarget(InitUtils::getInstance()->getRenderer(), NULL);
    SDL_RenderCopy(InitUtils::getInstance()->getRenderer(), baseTexture, NULL, NULL);

    SDL_RenderPresent(InitUtils::getInstance()->getRenderer());
    frameTime = SDL_GetTicks() - frameStart;
    if (frameDelay > frameTime) {
      SDL_Delay(frameDelay - frameTime);
    }
  }

  delete bare;
  delete wall;
  delete scoreSegments;
  delete ball;
  delete title;
  InitUtils::getInstance()->destroy();
  return 0;
}
