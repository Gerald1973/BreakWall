#include "SurpriseBrick.hpp"
#include "CustomEventUtils.hpp"
#include "InitUtils.hpp"

// Constructor
SurpriseBrick::SurpriseBrick() : Brick() {
}

// Destructor
SurpriseBrick::~SurpriseBrick() {}

// Initialize the surprise brick
void SurpriseBrick::init() {
SDL_Rect tmpRect;
  SDL_Texture *texture = InitUtils::getInstance()->getMapTextures()[GlobalConstants::BRICK_SURPRISE_TEXTURE_KEY];
  TextureWithPosition *textureWithPosition = new TextureWithPosition(texture, tmpRect);
  setTextureWithPosition(textureWithPosition);
  setValue(50); // Higher score for surprise brick
  fallSpeed = 5.0f;
  isFalling = false;
}

// Handle SDL events
void SurpriseBrick::performEvent(SDL_Event &event) {
  if (isDestroyed()) {
    return;
  }
  // Check for brick touched event
  Ball *ball = (Ball *)event.user.data1;
  if (isTouchedByBall(ball)) {
    startFalling();
  }
}

// Start falling when hit
void SurpriseBrick::startFalling() {
  if (!isFalling) {
    isFalling = true;
    // Play sound
    if (getSound()) {
      Mix_PlayChannel(-1, getSound(), 0);
    }
  }
}

// Update falling position
void SurpriseBrick::updatePosition() {
  if (isFalling && !isDestroyed()) {
  	TextureWithPosition *textureWithPosition = getTextureWithPosition();
	if (textureWithPosition->getY2() > GlobalConstants::BALL_ZONE_HEIGHT) {
    	setDestroyed(true);
  	} else {
  		textureWithPosition->setY(textureWithPosition->getY() + static_cast<int>(fallSpeed));
	}
  }
}

bool SurpriseBrick::isCaughtByBare(Bare *bare) {
  if (!isFalling || isDestroyed()) {
    return false;
  }
  SDL_Rect brickRect = getTextureWithPosition()->getPosition();
  SDL_Rect bareRect = bare->getTextureWithPosition()->getPosition();
  return SDL_HasIntersection(&brickRect, &bareRect);
}

// Render the brick
void SurpriseBrick::render() {
  if (!isDestroyed()) {
    if (isFalling) {
      updatePosition();
    }
    Brick::render();
  }
}