#ifndef WALL_H
#define WALL_H
#include <SDL2/SDL.h>
#include <vector>
#include "Brick.h"
#include "GlobalConstants.h"
#include "UtilConstants.h"

class Wall {
public:
	Wall();
	virtual ~Wall();
	std::vector<Brick*> getBricks();
	std::string getSongFileName();
	std::string getSongKey();
	void setBricks(std::vector<Brick*> bricks);
	void setSongFileName(std::string songFileName);
	void setSongKey(std::string songKey);
	void performEvent(SDL_Event &event);
	void playSong();
	int findIndex(Brick *bricks);
	void build();
	void render();
	void init();
	SDL_AudioDeviceID getSdlAudioDeviceId();
	SDL_AudioDeviceID initSong();
	void stopSong();
	void printStatus(SDL_AudioDeviceID dev);
protected:
private:
	std::vector<Brick*> bricks;
	std::string songFileName;
	std::string songKey;
};

#endif // WALL_H
