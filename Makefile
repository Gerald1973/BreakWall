SHELL         = /bin/sh
CXX           = g++
SDL2CFLAGS    = $(shell sdl2-config --cflags --libs)
CXXFLAGS      = -std=c++17 -Wall -Wuninitialized -g $(SDL2CFLAGS) -lSDL2_mixer -lSDL2_image
CXXFLAGS_DIST = -Wall -Wuninitialized -std=c++17 $(SDL2CFLAGS) -lSDL2_mixer -lSDL2_image -Ofast
INCLUDE       = -Iinclude/
SRC           = $(wildcard src/*.cpp)
OBJ_DIR       = obj
OBJECTS       = $(subst src/,$(OBJ_DIR)/,$(SRC:.cpp=.o))
EXE           = breakwall

# Paths for Windows
INCLUDE_PATHS_WIN = -ID:/programmation/tools/c/mingw/SDL2/i686-w64-mingw32/include
LIBRARY_PATHS_WIN = -LD:/programmation/tools/c/mingw/SDL2/i686-w64-mingw32/lib -LD:/programmation/tools/c/mingw/SDL2_MIXER/i686-w64-mingw32/lib
LINKER_FLAGS_WIN  = -lmingw32 -lSDL2main -lSDL2 -lSDL2_mixer -lSDL2_image

# Paths for Linux
INCLUDE_PATHS_LINUX = -I/usr/include
LIBRARY_PATHS_LINUX = -L/lib/x86_64-linux-gnu
LINKER_FLAGS_LINUX = -lSDL2main -lSDL2 -lSDL2_mixer -lSDL2_image -lm

# Compiler flags
COMPILER_FLAGS = -Wall -Wuninitialized

# Default target
all: compile

# Compile for Linux
compile: $(OBJECTS)
	@echo "File sources : $(SRC)"
	@echo "Target       : $@"
	$(CXX) $(OBJECTS) $(INCLUDE_PATHS_LINUX) $(LIBRARY_PATHS_LINUX) $(COMPILER_FLAGS) -g $(LINKER_FLAGS_LINUX) -o $(EXE)

# Object file compilation
$(OBJ_DIR)/%.o: src/%.cpp | $(OBJ_DIR)
	$(CXX) $(CXXFLAGS) $(INCLUDE) -c $< -o $@

# Create object directory
$(OBJ_DIR):
	mkdir -p $(OBJ_DIR)

# Compile for Windows
all_windows: clean_windows
	@echo "File sources : $(SRC)"
	@echo "Target       : $@"
	$(CXX) $(SRC) $(INCLUDE_PATHS_WIN) $(LIBRARY_PATHS_WIN) $(COMPILER_FLAGS) -g $(LINKER_FLAGS_WIN) -o $(EXE).exe

# Clean Windows build
clean_windows:
	rm -f $(EXE).exe

# Distribution build for Linux
dist:
	@echo "File sources : $(SRC)"
	@echo "Target       : $@"
	$(CXX) $(SRC) $(CXXFLAGS_DIST) $(INCLUDE) -o $(EXE)

# Windows distribution
windows_dist_all: windows_dist_compile
	mv $(EXE).exe dist

windows_dist_compile: windows_dist_prepare
	@echo "File sources : $(SRC)"
	@echo "Target       : $@"
	$(CXX) $(SRC) $(INCLUDE_PATHS_WIN) $(LIBRARY_PATHS_WIN) $(COMPILER_FLAGS) $(LINKER_FLAGS_WIN) -o $(EXE).exe

windows_dist_prepare:
	@echo "File sources : $(SRC)"
	@echo "Target       : $@"
	rm -rf dist
	mkdir dist
	cp windows/dll/* dist
	cp -r resources dist

# Clean Linux build
clean:
	rm -f $(EXE) $(OBJECTS)
	rm -rf $(OBJ_DIR)

# Phony targets
.PHONY: all compile all_windows clean_windows dist windows_dist_all windows_dist_compile windows_dist_prepare clean