/*
 * Wall002.hpp
 *
 *  Created on: 4 janv. 2021
 *      Author: Papa
 */

#ifndef INCLUDE_WALL002_HPP_
#define INCLUDE_WALL002_HPP_

#include "Wall.hpp"

class Wall002: public Wall {
public:
	void init();

	static Wall002* create() {
		return new Wall002();
	}
};

#endif /* INCLUDE_WALL002_HPP_ */
