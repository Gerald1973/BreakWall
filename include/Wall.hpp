#ifndef WALL_H
#define WALL_H
#include <SDL2/SDL.h>
#include <vector>
#include "Brick.h"
#include "GlobalConstants.h"
#include "UtilConstants.h"
#include <string>

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
	virtual void performEvent(SDL_Event &event);
	void playSong();
	int findIndex(Brick *bricks);
	void render();
	virtual void init();
	void initSong(std::string key);
	void resetSong();
	void stopSong();
	static Wall* create(){
		return new Wall();
	}
protected:
private:
	std::vector<Brick*> bricks;
	std::string songFileName;
	std::string songKey;
};

#endif // WALL_H
