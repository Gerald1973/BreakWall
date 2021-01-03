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
    static inline std::string BALL_TEXTURE_KEY= "ball";
    //sound key
    static inline std::string SOUND_KEY = "brick";
    static inline std::string SOUND_KEY_BALL_DEAD="sound_key_ball_dead";
protected:
private:
};

#endif // GLOBALCONSTANTS_H
