#include "../include/CustomEventUtils.hpp"

#include <iostream>
#include <string>
#include <SDL2/SDL.h>
#include "../include/InitUtils.hpp"

using namespace std;

void CustomEventUtils::postEventSongStop(string *message) {
	SDL_Event event;
	event.type = myEventType;
	event.user.code = SONG_STOP;
	event.user.data1 = (void*) message;
	event.user.data2 = nullptr;
	event.user.windowID = SDL_GetWindowID(InitUtils::getInstance()->getPWindow());
	SDL_PushEvent(&event);
}

void CustomEventUtils::postEventBrickRemaining(Wall *wall) {
	SDL_Event event;
	event.type = myEventType;
	event.user.code = BRICK_REMAINING;
	event.user.data1 = (void *) wall;
	event.user.data2 = nullptr;
	event.user.windowID = SDL_GetWindowID(InitUtils::getInstance()->getPWindow());
	SDL_PushEvent(&event);

}

void CustomEventUtils::postEventBrickRemoved(Brick *brick) {
	SDL_Event event;
	event.type = myEventType;
	event.user.code = BRICK_REMOVED;
	event.user.data1 = (void*) brick;
	event.user.data2 = nullptr;
	event.user.windowID = SDL_GetWindowID(InitUtils::getInstance()->getPWindow());
	SDL_PushEvent(&event);

}

void CustomEventUtils::postEventBrickTouched(Brick *brick) {
	SDL_Event event;
	event.type = SDL_USEREVENT;
	event.user.code = BRICK_TOUCHED;
	event.user.data1 = (void*) brick;
	event.user.data2 = nullptr;
	event.user.windowID = SDL_GetWindowID(InitUtils::getInstance()->getPWindow());
	SDL_PushEvent(&event);

}

void CustomEventUtils::postEventBorderTouched(Code code, Ball* ball) {
	SDL_Event event;
	event.type = myEventType;
	event.user.code = code;
	event.user.data1 = ball;
	event.user.data2 = nullptr;
	event.user.windowID = SDL_GetWindowID(InitUtils::getInstance()->getPWindow());
	SDL_PushEvent(&event);

}

CustomEventUtils::CustomEventUtils() {
	myEventType = SDL_RegisterEvents(1);
	if (myEventType != ((Uint32) -1)) {
		cout << "SDL user event correctly registered." << endl;
	} else {
		cout << "ERROR: SDL user event correctly registered." << endl;
	}
	cout << "DEBUG CustomEventUtils creation." << endl;
	cout << "DEBUG myEventType." << myEventType << endl;
}

void CustomEventUtils::postEventLiveFinished() {
	SDL_Event event;
	event.type = myEventType;
	event.user.code = LIVE_FINISHED;
	event.user.data1 = nullptr;
	event.user.data2 = nullptr;
	event.user.windowID = SDL_GetWindowID(InitUtils::getInstance()->getPWindow());
	SDL_PushEvent(&event);
}

void CustomEventUtils::postEventBareTouched() {
	SDL_Event event;
	event.type = myEventType;
	event.user.code = BARE_TOUCHED;
	event.user.data1 = nullptr;
	event.user.data2 = nullptr;
	event.user.windowID = SDL_GetWindowID(InitUtils::getInstance()->getPWindow());
	SDL_PushEvent(&event);
}

CustomEventUtils::~CustomEventUtils() {
	cout << "DEBUG CustomEventUtils destruction." << endl;
	this->instance = nullptr;
}

void CustomEventUtils::postEventBallMoved(Ball *ball) {
	SDL_Event event;
	event.type = myEventType;
	event.user.code = BALL_MOVED;
	event.user.data1 = (void*) ball;
	event.user.data2 = nullptr;
	event.user.windowID = SDL_GetWindowID(InitUtils::getInstance()->getPWindow());
	SDL_PushEvent(&event);
}
