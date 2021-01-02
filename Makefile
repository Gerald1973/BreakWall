SHELL         =/bin/sh
CXX           =g++
SDL2CFLAGS    = $(shell sdl2-config --cflags --libs)
CXXFLAGS      =-Wall -Wuninitialized -std=c++17 -g $(SDL2CFLAGS) -lSDL2_mixer -lSDL2_image -mfpu=neon
CXXFLAGS_DIST =-Wall -Wuninitialized -std=c++17 $(SDL2CFLAGS) -lSDL2_mixer -lSDL2_image -mfpu=neon
INCLUDE       = -Iinclude/
SRC           = $(wildcard ./src/*.cpp)
OBJECTS       = $(subst ./src/,$(OBJ_DIR)/, $(SRC:.cpp=.o))
EXE           = breakwall

#INCLUDE_PATHS specifies the additional include paths we'll need
INCLUDE_PATHS = -ID:\programmation\tools\c\mingw\SDL2\i686-w64-mingw32\include

#LIBRARY_PATHS specifies the additional library paths we'll need
LIBRARY_PATHS = -LD:\programmation\tools\c\mingw\SDL2\i686-w64-mingw32\lib -LD:\programmation\tools\c\mingw\SDL2_MIXER\i686-w64-mingw32\lib

#COMPILER_FLAGS specifies the additional compilation options we're using
# -w suppresses all warnings
# -Wl,-subsystem,windows gets rid of the console window
COMPILER_FLAGS = -Wall -Wuninitialized -std=c++17

#LINKER_FLAGS specifies the libraries we're linking against
LINKER_FLAGS = -lmingw32 -lSDL2main -lSDL2 -lSDL2_mixer -lSDL2_image


all: compile

compile:
	@echo "File sources : $(SRC)"
	@echo "Target       : $@"
	$(CXX) $(CXXFLAGS) $(INCLUDE) -o $(EXE) $(SRC)
	
all_windows: clean_windows
	@echo "File sources : $(SRC)"
	@echo "Target       : $@"
	$(CXX) $(SRC) $(INCLUDE_PATHS) $(LIBRARY_PATHS) $(COMPILER_FLAGS) -g $(LINKER_FLAGS) -o $(EXE).exe

clean_windows:
	rm -f $(EXE).exe

dist:
	@echo "File sources : $(SRC)"
	@echo "Target       : $@"
	$(CXX) $(CXXFLAGS_DIST) $(INCLUDE) -o $(EXE) $(SRC)


windows_dist_all: windows_dist_compile
	mv $(EXE).exe dist
	
windows_dist_compile: windows_dist_prepare
	@echo "File sources : $(SRC)"
	@echo "Target       : $@"
	$(CXX) $(SRC) $(INCLUDE_PATHS) $(LIBRARY_PATHS) $(COMPILER_FLAGS) $(LINKER_FLAGS) -o $(EXE).exe
	
windows_dist_prepare:
	@echo "File sources : $(SRC)"
	@echo "Target       : $@"
	rm -rf dist
	mkdir dist
	cp windows/dll/* dist
	cp -r resources dist 
	
	


.PHONY: all mkdirs clean

clean:
	rm $(EXE).exe