// src/ExplodingBrick.cpp
#include "../include/ExplodingBrick.hpp"
#include "../include/InitUtils.hpp"
#include "../include/CustomEventUtils.hpp"
#include "../include/GlobalConstants.h"
#include <cmath>
#include <iostream>

// Define static member
const float ExplodingBrick::FRAGMENT_SIZE = 16.0f; // 16x16 pixels for fragments

ExplodingBrick::ExplodingBrick() : Brick() {
    isExploding = false;
    fragmentSpeed = 5.0f; // 5 pixels per frame
}

ExplodingBrick::~ExplodingBrick() {
    std::cout << "DEBUG: ExplodingBrick destruction." << std::endl;
}

void ExplodingBrick::init() {
    SDL_Rect tmpRect;
	TextureWithPosition *textureWithPosition = new TextureWithPosition(InitUtils::getInstance()->getMapTextures()[GlobalConstants::EXPLODING_BRICK_TEXTURE_KEY], tmpRect);
	setTextureWithPosition(textureWithPosition);
    isExploding = false;
    fragments.clear();
}

void ExplodingBrick::performEvent(SDL_Event &event) {
    if (event.user.code == CustomEventUtils::Code::BALL_MOVED && !isDestroyed() && !isExploding) {
        Ball *ball = (Ball*)event.user.data1;
        if (this->isTouchedByBall(ball)) {
            this->bounces(ball); // Apply bounce to ball
            this->setDestroyed(true); // Prevent further collisions
            startExplosion(); // Start the explosion
            CustomEventUtils::getInstance()->postEventBrickTouched(this);
        }
    }
}

void ExplodingBrick::render() {
    if (!isDestroyed() && !isExploding) {
        Brick::render(); // Render normally if not exploding
        return;
    }

    if (isExploding) {
        updateFragments(); // Update fragment positions

        // Render each active fragment
        SDL_Renderer *renderer = InitUtils::getInstance()->getRenderer();
        for (const auto& fragment : fragments) {
            if (fragment.active) {
                SDL_RenderCopy(renderer, getTextureWithPosition()->getTexture(), nullptr, &fragment.position);
            }
        }

        // Check if all fragments are inactive
        bool allInactive = true;
        for (const auto& fragment : fragments) {
            if (fragment.active) {
                allInactive = false;
                break;
            }
        }
        if (allInactive) {
            isExploding = false;
            CustomEventUtils::getInstance()->postEventBrickRemoved(this); // Remove brick
        }
    }
}

void ExplodingBrick::startExplosion() {
    isExploding = true;
    fragments.clear();

    // Get brick's center and size
    int centerX = getTextureWithPosition()->getAbsCenterX();
    int centerY = getTextureWithPosition()->getAbsCenterY();
    int fragmentW = static_cast<int>(FRAGMENT_SIZE);
    int fragmentH = static_cast<int>(FRAGMENT_SIZE);

    // Define 8 directions (angles in radians: 0, 45, 90, 135, 180, 225, 270, 315 degrees)
    const float angles[] = {0.0f, M_PI/4, M_PI/2, 3*M_PI/4, M_PI, 5*M_PI/4, 3*M_PI/2, 7*M_PI/4};

    // Create 8 fragments
    for (int i = 0; i < NUM_FRAGMENTS; ++i) {
        Fragment fragment;
        fragment.position = {centerX - fragmentW/2, centerY - fragmentH/2, fragmentW, fragmentH};
        fragment.dirX = std::cos(angles[i]); // Normalized direction x
        fragment.dirY = std::sin(angles[i]); // Normalized direction y
        fragment.active = true;
        fragments.push_back(fragment);
    }
}

void ExplodingBrick::updateFragments() {
    for (auto& fragment : fragments) {
        if (!fragment.active) continue;

        // Update position
        fragment.position.x += static_cast<int>(fragment.dirX * fragmentSpeed);
        fragment.position.y += static_cast<int>(fragment.dirY * fragmentSpeed);

        // Check if fragment is out of bounds
        if (fragment.position.x <= 0 || fragment.position.x + fragment.position.w >= GlobalConstants::BALL_ZONE_WIDTH ||
            fragment.position.y <= 0 || fragment.position.y + fragment.position.h >= GlobalConstants::BALL_ZONE_HEIGHT) {
            fragment.active = false;
        }
    }
}