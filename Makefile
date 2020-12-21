SHELL = /bin/sh
CXX         :=g++
SDL2CFLAGS  := $(shell sdl2-config --cflags --libs)
CXXFLAGS    :=-Wall -Wuninitialized -std=c++17 -g $(SDL2CFLAGS) -lSDL2_mixer -lSDL2_image -mfpu=neon
LDFLAGS     := -L/usr/lib -Llib $(SDL2LDFLAGS)
BUILD       := ./build
OBJ_DIR     := $(BUILD)/objects
INCLUDE     := -Iinclude/
SRC         := $(wildcard ./src/*.cpp)
OBJECTS     := $(subst ./src/,$(OBJ_DIR)/, $(SRC:.cpp=.o))
EXE = breakwall
# sources=$(src)/Ball.cpp $(src)/Bare.cpp $(src)/Brick.cpp $(src)/GlobalConstants.cpp $(src)/main.cpp $(src)/ScoreSegments.cpp $(src)/TextureWithPosition.cpp $(src)/UtilConstants.cpp $(src)/Wall.cpp
# headers=$(src)/Ball.h $(src)/Bare.h $(src)/Brick.h $(src)/GlobalConstants.h $(src)/ScoreSegments.h $(src)/TextureWithPosition.h $(src)/UtilConstants.h $(src)/Wall.h
all: mkdirs compile

compile:
	@echo "File sources : $<"
	@echo "Destination  : $@"
	$(CXX) $(CXXFLAGS) $(INCLUDE) -o breakwall.exe $(SRC)

.PHONY: all mkdirs clean

mkdirs:
	mkdir -p $(OBJ_DIR)

clean:
	rm -rvf $(BUILD)/*