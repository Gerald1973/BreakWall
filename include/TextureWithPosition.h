#ifndef TEXTUREWITHPOSITION_H
#define TEXTUREWITHPOSITION_H
#include <iostream>
#include <SDL2/SDL.h>


class TextureWithPosition
{
    public:
        TextureWithPosition(SDL_Texture* texture, SDL_Rect position);
        TextureWithPosition(SDL_Texture* texture, int x, int y, int width, int height);
        virtual ~TextureWithPosition();
        const SDL_Rect & getPosition();
        const SDL_Rect & getOriginRect();
        SDL_Texture* getTexture();
        void setTexture(SDL_Texture* sdlTexture);
        void setX(int x);
        void setY(int y);
        int getX();
        int getY();
        int getX2();
        int getY2();
        int getAbsCenterX();
        int getAbsCenterY();
    protected:
    private:
        SDL_Rect position;
        SDL_Rect originRect;
        SDL_Texture* texture;
};

#endif // TEXTUREWITHPOSITION_H
