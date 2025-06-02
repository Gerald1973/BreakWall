#include "../include/Wall004.hpp"
#include "../include/Brick.h"
#include "../include/ExplodingBrick.hpp"
#include "../include/GlobalConstants.h"
#include "../include/InitUtils.hpp"
#include "../include/MissileBrick.hpp"
#include "../include/MovingRotatingBrick.hpp"
#include <vector>

// Initialize Wall004 with a centered 'S' pattern using all brick types
void Wall004::init() {
  std::vector<Brick *> bricks;
  // Define the 7x7 'S' pattern
  int s_pattern[7][7] = {
      {2, 2, 2, 2, 2, 2, 2}, // ExplodingBrick
      {2, 0, 0, 0, 0, 0, 0}, // Brick
      {2, 0, 0, 0, 0, 0, 0}, // Brick
      {1, 1, 1, 1, 1, 1, 1}, // MovingRotatingBrick
      {0, 0, 0, 0, 0, 0, 1}, // Brick
      {0, 0, 0, 0, 0, 0, 1}, // Brick
      {3, 3, 3, 3, 3, 3, 3}  // MissileBrick
  };
  this->background = new Background("resources/images/steampunkcomputer.jpg");
  // Wall dimensions
  const int brickWidth = 64;
  const int brickHeight = 32;
  const int patternWidth = 7;  // 7 bricks wide
  const int patternHeight = 7; // 7 bricks tall
  const int wallWidth = 15;    // Max 15 bricks
  const int wallHeight = 9;    // Max 9 bricks to fit WALL_ZONE_HEIGHT
  // Create the wall
  for (int y = 0; y < wallHeight; ++y) {
    for (int x = 0; x < wallWidth; ++x) {
      Brick *brick = nullptr;
      // Check if the position is within the 7x7 pattern
      int patternX = x - (wallWidth - patternWidth) / 2; // Center pattern
      int patternY = y - (wallHeight - patternHeight) / 2;
      if (patternX >= 0 && patternX < patternWidth && patternY >= 0 && patternY < patternHeight) {
        // Inside the 'S' pattern
        switch (s_pattern[patternY][patternX]) {
        case 1:
          brick = new MovingRotatingBrick();
          break;
        case 2:
          brick = new ExplodingBrick();
          break;
        case 3:
          brick = new MissileBrick();
          break;
        case 0:
        default:
          brick = new Brick();
          break;
        }
      } else {
        brick = new Brick();
      }
      // Set brick properties
      brick->init();
      brick->getTextureWithPosition()->setX(GlobalConstants::WALL_ZONE_X + x * brickWidth);
      brick->getTextureWithPosition()->setY(GlobalConstants::WALL_ZONE_Y + y * brickHeight);
      bricks.push_back(brick);
    }
  }

  // Set the bricks and song
  setBricks(bricks);
  initSong("Wall004");
}