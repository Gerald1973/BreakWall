#ifndef MISSILE_BRICK_HPP
#define MISSILE_BRICK_HPP

#include <SDL2/SDL.h>
#include "Brick.h"
#include "Ball.h"

class MissileBrick : public Brick {
public:
    MissileBrick();
    virtual ~MissileBrick();
    void performEvent(SDL_Event &event) override;
    void render() override;
    void init() override;

private:
    bool isMissileActive;
    float missileDirX; // Direction x normalisée du missile
    float missileDirY; // Direction y normalisée du missile
    float missileSpeed; // Vitesse du missile en pixels par frame
    static const float MISSILE_SIZE; // Taille du missile (carré)

    void launchMissile(Ball *ball);
    void updateMissile();
};

#endif // MISSILE_BRICK_HPP