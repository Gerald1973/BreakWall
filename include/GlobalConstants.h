#ifndef GLOBALCONSTANTS_H
#define GLOBALCONSTANTS_H
#include <string>

class GlobalConstants
{
public:
    static inline int SCREEN_WIDTH = 960;
    static inline int SCREEN_HEIGHT = 540;
    static inline int SEVEN_SEGMENT_WIDTH = 48;
    static inline int SEVEN_SEGMENT_HEIGHT = 64;
    static inline int BALL_ZONE_X=0;
    static inline int BALL_ZONE_Y=0;
    static inline int BALL_ZONE_WIDTH=960;
    static inline int BALL_ZONE_HEIGHT=476;
    static inline int WALL_ZONE_X=0;
    static inline int WALL_ZONE_Y=0;
    static inline int WALL_ZONE_WIDTH=960;
    static inline int WALL_ZONE_HEIGHT=284;
    //Brick texture key
    static inline std::string TEXTURE_KEY = "brick";
	static inline std::string EXPLODING_BRICK_TEXTURE_KEY = "explodingbrick";
	static inline std::string ROTATING_BRICK_TEXTURE_KEY = "rotatingbrick";
    static inline std::string BALL_TEXTURE_KEY= "ball";
    //sound key
    static inline std::string BRICK_SOUND_KEY = "brick_key";
    static inline std::string BALL_DEAD_SOUND_KEY="sound_key_ball_dead";
    static inline std::string BARE_SOUND_KEY="bare_sound_key";
    //Wall
protected:
private:
};

#endif // GLOBALCONSTANTS_H
