// src/MovingRotatingBrick.cpp
#include "../include/MovingRotatingBrick.hpp"
#include "../include/InitUtils.hpp"
#include "../include/CustomEventUtils.hpp"
#include "../include/GlobalConstants.h"
#include <cmath>
#include <iostream>
#include <algorithm>

MovingRotatingBrick::MovingRotatingBrick() : Brick() {
    isMoving = false;
    moveDirX = 0.0f;
    moveDirY = 0.0f;
    moveSpeed = 5.0f; // 5 pixels per frame
    rotationAngle = 0.0f;
    rotationSpeed = 10.0f; // 5 degrees per frame
}

MovingRotatingBrick::~MovingRotatingBrick() {
    std::cout << "DEBUG: MovingRotatingBrick destruction." << std::endl;
}

void MovingRotatingBrick::init() {
	Brick::init(); // Call base class init
    if (!getTextureWithPosition()) {
        std::cerr << "ERROR: textureWithPosition is null in MovingRotatingBrick::init" << std::endl;
        return;
    }
    isMoving = false;
    rotationAngle = 0.0f;
}

void MovingRotatingBrick::performEvent(SDL_Event &event) {
    if (event.user.code == CustomEventUtils::Code::BALL_MOVED && !isDestroyed() && !isMoving) {
        Ball *ball = (Ball*)event.user.data1;
        if (this->isTouchedByBall(ball)) {
            this->bounces(ball); // Apply bounce to ball
            this->setDestroyed(true); // Prevent further collisions
            startMoving(); // Start moving toward nearest edge
            CustomEventUtils::getInstance()->postEventBrickTouched(this);
        }
    }
}

void MovingRotatingBrick::render() {
    if (!isDestroyed()) {
        Brick::render(); // Render normally if not moving
        return;
    }

    if (isMoving) {
        updatePosition(); // Update position and rotation

        // Render with rotation using SDL_RenderCopyEx
        SDL_Renderer *renderer = InitUtils::getInstance()->getRenderer();
        SDL_Rect *destRect = const_cast<SDL_Rect*>(&getTextureWithPosition()->getPosition());
        SDL_RenderCopyEx(renderer, getTextureWithPosition()->getTexture(), nullptr, destRect,
                         rotationAngle, nullptr, SDL_FLIP_NONE);

        // Check if brick has reached the edge
        int x = getTextureWithPosition()->getX();
        int y = getTextureWithPosition()->getY();
        int w = getTextureWithPosition()->getPosition().w;
        int h = getTextureWithPosition()->getPosition().h;

        if (x <= 0 || x + w >= GlobalConstants::BALL_ZONE_WIDTH ||
            y <= 0 || y + h >= GlobalConstants::BALL_ZONE_HEIGHT) {
            isMoving = false; // Stop moving
            CustomEventUtils::getInstance()->postEventBrickRemoved(this); // Remove brick
        }
    }
}

void MovingRotatingBrick::startMoving() {
    isMoving = true;
    determineNearestEdge();
}

void MovingRotatingBrick::determineNearestEdge() {
    // Get brick's center
    int centerX = getTextureWithPosition()->getAbsCenterX();
    int centerY = getTextureWithPosition()->getAbsCenterY();

    // Calculate distances to each edge
    float distLeft = centerX; // Distance to x=0
    float distRight = GlobalConstants::BALL_ZONE_WIDTH - centerX; // Distance to x=960
    float distTop = centerY; // Distance to y=0
    float distBottom = GlobalConstants::BALL_ZONE_HEIGHT - centerY; // Distance to y=476

    // Find minimum distance
    float minDist = std::min({distLeft, distRight, distTop, distBottom});

    // Set direction based on nearest edge
    if (minDist == distLeft) {
        moveDirX = -1.0f; moveDirY = 0.0f; // Move left
    } else if (minDist == distRight) {
        moveDirX = 1.0f; moveDirY = 0.0f; // Move right
    } else if (minDist == distTop) {
        moveDirX = 0.0f; moveDirY = -1.0f; // Move up
    } else {
        moveDirX = 0.0f; moveDirY = 1.0f; // Move down
    }
}

void MovingRotatingBrick::updatePosition() {
    // Update position
    float newX = getTextureWithPosition()->getX() + moveDirX * moveSpeed;
    float newY = getTextureWithPosition()->getY() + moveDirY * moveSpeed;
    getTextureWithPosition()->setX(static_cast<int>(newX));
    getTextureWithPosition()->setY(static_cast<int>(newY));

    // Update rotation
    rotationAngle += rotationSpeed;
    if (rotationAngle >= 360.0f) {
        rotationAngle -= 360.0f; // Keep angle in [0, 360)
    }
}