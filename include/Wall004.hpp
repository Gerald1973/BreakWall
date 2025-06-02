#ifndef INCLUDE_WALL004_HPP_
#define INCLUDE_WALL004_HPP_

#include "Wall.hpp"

class Wall004 : public Wall
{
public:
	void init() override;

	static Wall004 *create()
	{
		return new Wall004();
	}
};

#endif /* INCLUDE_WALL004_HPP_ */