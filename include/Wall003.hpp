// include/Wall003.hpp
#ifndef INCLUDE_WALL003_HPP_
#define INCLUDE_WALL003_HPP_

#include "Wall.hpp"

class Wall003 : public Wall {
public:
    void init();

    static Wall003* create() {
        return new Wall003();
    }
};

#endif /* INCLUDE_WALL003_HPP_ */