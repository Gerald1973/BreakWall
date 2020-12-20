SHELL = /bin/sh
CXX       :=g++
CXXFLAGS :=-Wall -Wuninitialized -std=c++17 -g
LDFLAGS  := -L/usr/lib -Llib
BUILD    := ./build
OBJ_DIR  := $(BUILD)/objects
INCLUDE  := -Iinclude/
SRC      := $(wildcard src/*.cpp)
OBJECTS  := $(SRC:%.cpp=$(OBJ_DIR)/%.o)
# sources=$(src)/Ball.cpp $(src)/Bare.cpp $(src)/Brick.cpp $(src)/GlobalConstants.cpp $(src)/main.cpp $(src)/ScoreSegments.cpp $(src)/TextureWithPosition.cpp $(src)/UtilConstants.cpp $(src)/Wall.cpp
# headers=$(src)/Ball.h $(src)/Bare.h $(src)/Brick.h $(src)/GlobalConstants.h $(src)/ScoreSegments.h $(src)/TextureWithPosition.h $(src)/UtilConstants.h $(src)/Wall.h
all: mkdirs $(BUILD)/program

$(OBJ_DIR)/%.o: %.cpp
	@echo "$(SRC)"
	@echo "Call gcc to generate $@ from $<"
	$(CXX) $(CXXFLAGS) $(INCLUDE) -c $< -o $(OBJ_DIR)/$(notdir $@)

$(BUILD)/program: $(OBJECTS)
	@echo "10"
	$(CXX) $(CXXFLAGS) -o $(APP_DIR)/$(TARGET) $(OBJ_DIR)/$(notdir $^) $(LDFLAGS)

.PHONY: all mkdirs clean

mkdirs:
	mkdir -p $(OBJ_DIR)

clean:
	rm -rvf $(BUILD)/*