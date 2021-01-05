#define REGISTER_PLUGIN
#define GLOBALCONSTANTS_H

#include <string>
#include <map>
#include <functional>
# include <iostream>
#include "Wall.hpp"

#define REGISTER_WALL(level, create_func) \
    WallRegistry::add(level, (create_func))

class WallRegistry {
public:

	typedef std::function<Wall* ()> FactoryFunction;
	typedef std::unordered_map<int, FactoryFunction> FactoryMap;

	static bool add(int level, FactoryFunction fac) {
		bool result = false;
		FactoryMap& map = getFactoryMap();
		if (map.find(level) != map.end()) {
			std::cout << "DEBUG: level " << level << " not added." << std::endl;
		} else {
			getFactoryMap()[level] = fac;
			result = true;
		}
		return result;
	}

	static Wall* create(int level) {
		auto map = getFactoryMap();
		Wall *result = nullptr;
		if (map.find(level) == map.end()) {
			std::cout << "DEBUG: level " << level << " not found." << std::endl;
		} else {
			std::cout << "DEBUG: level " << level << " found." << std::endl;
			result = map[level]();
		}
		return result;
	}

private:
	// Use Meyer's singleton to prevent SIOF
	static FactoryMap& getFactoryMap() {
		static FactoryMap map;
		return map;
	}
};
