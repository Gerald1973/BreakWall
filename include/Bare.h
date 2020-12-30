#ifndef BARE_H
#define BARE_H
#include <SDL2/SDL.h>
#include <SDL2/SDL_mixer.h>

#include "TextureWithPosition.h"

class Bare
{
    public:
        Bare();
        virtual ~Bare();
        void setTextureWithPosition(TextureWithPosition* textureWithPosition);
        TextureWithPosition* getTextureWithPosition();
        void render();
        Mix_Chunk* getSound();
        void setSound(Mix_Chunk* sound);
    protected:
    private:
        TextureWithPosition* textureWithPosition;
        Mix_Chunk* sound;

};

#endif // BARE_H
