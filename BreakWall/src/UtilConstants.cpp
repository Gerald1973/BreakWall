#include "UtilConstants.h"

UtilConstants *UtilConstants::instance = NULL;

UtilConstants::UtilConstants()
{
    this->screenSize.h=540;
    this->screenSize.w=960;
    this->screenSize.x=0;
    this->screenSize.y=0;
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

