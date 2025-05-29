#ifndef MOVING_ROTATING_BRICK_H
#define MOVING_ROTATING_BRICK_H

#include <SDL2/SDL.h>
#include "Brick.h"
#include "Ball.h"

class MovingRotatingBrick : public Brick {
public:
    MovingRotatingBrick();
    virtual ~MovingRotatingBrick();
    void performEvent(SDL_Event &event) override;
    void render() override;
    void init() override;

private:
    bool isMoving;
    float moveDirX; // Direction vector x (normalized)
    float moveDirY; // Direction vector y (normalized)
    float moveSpeed; // Pixels per frame
    float rotationAngle; // Current rotation in degrees
    float rotationSpeed; // Degrees per frame
    void startMoving();
    void updatePosition();
    void determineNearestEdge();
};

#endif // MOVING_ROTATING_BRICK_H