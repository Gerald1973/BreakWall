SHELL = /bin/sh
CXX           :=g++
SDL2CFLAGS    := $(shell sdl2-config --cflags --libs)
CXXFLAGS      :=-Wall -Wuninitialized -std=c++17 -g $(SDL2CFLAGS) -lSDL2_mixer -lSDL2_image -mfpu=neon
CXXFLAGS_DIST :=-Wall -Wuninitialized -std=c++17 $(SDL2CFLAGS) -lSDL2_mixer -lSDL2_image -mfpu=neon
INCLUDE       := -Iinclude/
SRC           := $(wildcard ./src/*.cpp)
OBJECTS       := $(subst ./src/,$(OBJ_DIR)/, $(SRC:.cpp=.o))
EXE           := breakwall

all: compile

compile:
	@echo "File sources : $(SRC)"
	@echo "Target       : $@"
	$(CXX) $(CXXFLAGS) $(INCLUDE) -o $(EXE).exe $(SRC)

dist:
	@echo "File sources : $(SRC)"
	@echo "Target       : $@"
	$(CXX) $(CXXFLAGS_DIST) $(INCLUDE) -o $(EXE) $(SRC)

.PHONY: all mkdirs clean

clean:
	rm $(EXE).exe