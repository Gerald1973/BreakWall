#ifndef UTILCONSTANTS_H
#define UTILCONSTANTS_H
#include <SDL2/SDL.h>

class UtilConstants
{
    public:
        SDL_Rect gameZone;
        static UtilConstants* getInstance();
        ~UtilConstants();
    private:
        UtilConstants();
        static UtilConstants* instance;
};

#endif // CONSTANTS_H
