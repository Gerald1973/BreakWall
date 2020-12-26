#include <iostream>
#include <string>
#include "../include/Brick.h"

#define __CHANNELS__ 16
#define __FP_SHIFT__ 14
#define __FP_ONE__ 16384

class CustomEventUtils
{

public:
    enum Code
    {
        SONG_START = 0,
        SONG_STOP = 2,
        LEVEL_FINISHED = 3,
        BRICK_TOUCHED=40
    };

    void postEventSongStop(std::string *message);
    void postEventBrickTouched(Brick* brick);

    static inline CustomEventUtils *getInstance()
    {
        if (!instance)
        {
            instance = new CustomEventUtils();
        }
        return instance;
    }

private:
    static inline CustomEventUtils *instance = NULL;

    CustomEventUtils();
    ~CustomEventUtils();
};