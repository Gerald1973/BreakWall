// include/ExplodingBrick.hpp
#ifndef EXPLODING_BRICK_HPP
#define EXPLODING_BRICK_HPP

#include <SDL2/SDL.h>
#include <vector>
#include "Brick.h"
#include "Ball.h"

class ExplodingBrick : public Brick {
public:
    ExplodingBrick();
    virtual ~ExplodingBrick();
    void performEvent(SDL_Event &event) override;
    void render() override;
    void init() override;

private:
    struct Fragment {
        SDL_Rect position; // Position and size of the fragment
        float dirX; // Normalized direction x
        float dirY; // Normalized direction y
        bool active; // Whether the fragment is still on screen
    };

    bool isExploding;
    std::vector<Fragment> fragments;
    float fragmentSpeed; // Pixels per frame
    static const int NUM_FRAGMENTS = 8;
    static const float FRAGMENT_SIZE; // Size of each fragment (e.g., 16x16 pixels)

    void startExplosion();
    void updateFragments();
};

#endif // EXPLODING_BRICK_HPP