#ifndef SURPRISE_BRICK_HPP
#define SURPRISE_BRICK_HPP

#include <SDL2/SDL.h>
#include "Brick.h"
#include "Ball.h"
#include "Bare.hpp"
#include "GlobalConstants.h"

// Class representing a surprise brick that falls when hit
class SurpriseBrick : public Brick {
public:
    SurpriseBrick();
    virtual ~SurpriseBrick();
    
    // Override to handle falling behavior
    void performEvent(SDL_Event &event) override;
    
    // Override to render falling brick
    void render() override;
    
    // Initialize texture and properties
    void init() override;
    
    // Check collision with bare
    bool isCaughtByBare(Bare* bare);
    
private:
    // State of the brick (falling or not)
    bool isFalling;
    
    // Falling speed in pixels per frame
    float fallSpeed;
    
    // Start falling when hit by ball
    void startFalling();
    
    // Update falling position
    void updatePosition();
};
#endif 