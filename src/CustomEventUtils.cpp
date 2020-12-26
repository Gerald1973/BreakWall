#include "../include/CustomEventUtils.hpp"
#include <iostream>
#include <string>
#include <SDL2/SDL.h>

using namespace std;

void CustomEventUtils::postEventSongStop(string* message)
{
    Uint32 myEventType = SDL_RegisterEvents(1);
    if (myEventType != ((Uint32)-1))
    {
        SDL_Event event;
        event.type = SDL_USEREVENT;
        event.user.code = SONG_STOP;
        event.user.data1 = (void *) message;
        event.user.data2 = NULL;
        event.user.data2 = 0;
        SDL_PushEvent(&event);
    }
}

void CustomEventUtils::postEventBrickTouched(Brick* brick){
    Uint32 myEventType = SDL_RegisterEvents(1);
    if (myEventType != ((Uint32)-1))
    {
        SDL_Event event;
        event.type = SDL_USEREVENT;
        event.user.code = BRICK_TOUCHED;
        event.user.data1 = (void *) brick;
        event.user.data2 = NULL;
        event.user.data2 = 0;
        SDL_PushEvent(&event);
    }
}

CustomEventUtils::CustomEventUtils()
{
    cout << "DEBUG CustomEventUtils creation." << endl;
}

CustomEventUtils::~CustomEventUtils()
{
    cout << "DEBUG CustomEventUtils destruction." << endl;
    this->instance = NULL;
}
