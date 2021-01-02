#include <iostream>
#include <string>
#include "Ball.h"
#include <vector>
#include "../include/Wall.hpp"

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
		BRICK_REMOVED=41,
		BRICK_REMAINING=42,
        BORDER_TOP_TOUCHED=50,
        BORDER_RIGHT_TOUCHED=51,
        BORDER_BOTTOM_TOUCHED=52,
        BORDER_LEFT_BOTTOM_TOUCHED=53,
		LIVE_FINISHED=60,
		BARE_TOUCHED=70,
		BALL_MOVED=100
    };

    void postEventSongStop(std::string *message);
    void postEventBrickRemoved(Brick* brick);
    void postEventBrickTouched(Brick* brick);
    void postEventBrickRemaining(Wall* wall);
    void postEventBorderTouched(Code code);
    void postEventLiveFinished();
    void postEventBareTouched();
    void postEventBallMoved(Ball* ball);

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
