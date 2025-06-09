#ifndef WALL_H
#define WALL_H
#include <SDL2/SDL.h>
#include <vector>
#include "Brick.h"
#include "GlobalConstants.h"
#include "UtilConstants.h"
#include <string>
#include "Background.hpp"

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
	void explodeAllBricks();
protected:
	Background* background;
	struct StarFragment {
        SDL_Rect position; // Position and size of the fragment
        float dirX; // Normalized direction x
        float dirY; // Normalized direction y
        bool active; // Whether the fragment is still on screen
    };
	  // List of star fragments for explosion effect
    std::vector<StarFragment> starFragments;
    
    // Number of star fragments per brick
    static const int NUM_FRAGMENTS = 4;
    
    // Size of each star fragment
    static const float STAR_SIZE;
    
    // Speed of star fragments
    float starSpeed;
    
    // Create star fragments for a given brick position
    void createStarFragments(const SDL_Rect& brickPos);
    
    // Update star fragments
    void updateStarFragments();
private:
	std::vector<Brick*> bricks;
	std::string songFileName;
	std::string songKey;
};

#endif // WALL_H
