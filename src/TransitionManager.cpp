#include "../include/TransitionManager.hpp"
#include <cstdlib>
#include <ctime>

TransitionManager::TransitionManager(SDL_Renderer *renderer, int screenWidth, int screenHeight)
    : renderer(renderer), screenWidth(screenWidth), screenHeight(screenHeight) {}

void TransitionManager::startTransition() {
  // Capturer l'écran actuel
  capturedTexture = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_RGBA8888, SDL_TEXTUREACCESS_TARGET, screenWidth, screenHeight);
  SDL_SetRenderTarget(renderer, capturedTexture);
  SDL_RenderCopy(renderer, nullptr, nullptr, nullptr);
  SDL_SetRenderTarget(renderer, nullptr);

  // Initialiser les colonnes
  int NUM_COLUMNS = screenWidth / COLUMN_WIDTH;
  columnSpeeds.resize(NUM_COLUMNS);
  columnY.resize(NUM_COLUMNS, 0);
  srand(static_cast<unsigned>(time(0)));
  for (int i = 0; i < NUM_COLUMNS; ++i) {
    columnSpeeds[i] = static_cast<float>(rand() % 5 + 1);
  }
  animationFinished = false;
}

void TransitionManager::updateTransition() {
  if (!animationFinished) {
    animationFinished = true;
    for (size_t i = 0; i < columnY.size(); ++i) {
      columnY[i] += static_cast<int>(columnSpeeds[i]);
      if (columnY[i] < screenHeight) {
        animationFinished = false;
      }
    }
  }
}

void TransitionManager::renderTransition() {
  if (!animationFinished) {
    SDL_RenderClear(renderer);
    for (size_t i = 0; i < columnY.size(); ++i) {
      if (columnY[i] < screenHeight) {
        SDL_Rect srcRect = {static_cast<int>(i * COLUMN_WIDTH), 0, COLUMN_WIDTH, screenHeight};
        SDL_Rect dstRect = {static_cast<int>(i * COLUMN_WIDTH), columnY[i], COLUMN_WIDTH, screenHeight};
        SDL_RenderCopy(renderer, capturedTexture, &srcRect, &dstRect);
      }
    }
    SDL_RenderPresent(renderer);
  }
}

bool TransitionManager::isTransitionFinished() const { return animationFinished; }
