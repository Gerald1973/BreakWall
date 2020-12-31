#include <iostream>
#include <string>

#include "Brick.h"

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
        BRICK_TOUCHED=40,
        BORDER_TOP_TOUCHED=50,
        BORDER_RIGHT_TOUCHED=51,
        BORDER_BOTTOM_TOUCHED=52,
        BORDER_LEFT_BOTTOM_TOUCHED=53,
		LIVE_FINISHED=60
    };

    void postEventSongStop(std::string *message);
    void postEventBrickTouched(Brick* brick);
    void postEventBorderTouched(Code code);
    void postEventLiveFinished();

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
