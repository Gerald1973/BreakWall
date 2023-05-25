#include "../include/UtilConstants.h"

#include "../include/GlobalConstants.h"

UtilConstants *UtilConstants::instance = NULL;

UtilConstants::UtilConstants()
{
    this->gameZone.h=GlobalConstants::SCREEN_HEIGHT - GlobalConstants::SEVEN_SEGMENT_HEIGHT;
    this->gameZone.w=GlobalConstants::SCREEN_WIDTH;
    this->gameZone.x=0;
    this->gameZone.y=0;
}

UtilConstants::~UtilConstants()
{
    //dtor
}

UtilConstants* UtilConstants::getInstance(){
    if (instance == NULL ){
        instance = new UtilConstants();
    }
    return instance;
}

