#ifndef WALL_H
#define WALL_H
#include <SDL2/SDL.h>
#include <vector>
#include "Brick.h"
#include "GlobalConstants.h"
#include "UtilConstants.h"

class Wall
{
    public:
        Wall();
        virtual ~Wall();
        std::vector< std::vector<Brick*> > getBricks();
        void setBricks(std::vector<std::vector<Brick*> > bricks);
        void build();
        void render(SDL_Renderer *renderer);
    protected:
    private:
        std::vector< std::vector<Brick*> > bricks;
};

#endif // WALL_H
