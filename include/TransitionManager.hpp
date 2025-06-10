#include <SDL2/SDL.h>
#include <vector>

class TransitionManager {
public:
  TransitionManager(SDL_Renderer *renderer, int screenWidth, int screenHeight);
  void startTransition();
  void updateTransition();
  void renderTransition();
  bool isTransitionFinished() const;
 
private:
  SDL_Renderer *renderer;
  int screenWidth;
  int screenHeight;
  SDL_Texture *capturedTexture = nullptr;
  std::vector<float> columnSpeeds;
  std::vector<int> columnY;
  bool animationFinished = true;
  const int COLUMN_WIDTH = 1;
  bool transitioning = false;
};