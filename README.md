# BreakWall

## Overview

BreakWall is a classic brick-breaking game built with SDL2, featuring multiple levels, sound effects, and music inspired by the Commodore Amiga era. This document provides prerequisites, compilation instructions, packaging details, and element descriptions for the game.

## Prerequisites

To develop and run BreakWall, you need the following dependencies:

### For All Systems

- **SDL2** (version 2.0.14)  
  <https://www.libsdl.org/download-2.0.php>  
- **SDL2_mixer** (version 2.0.4)  
  <https://www.libsdl.org/projects/SDL_mixer/>  
- **SDL2_image** (version 2.0.5)  
  <https://www.libsdl.org/projects/SDL_image/>  

For assistance with compiling these SDL versions, refer to the script at:  
<https://github.com/Gerald1973/git_fetch_last_release>

### Additional Tools

- **Linux**: 
  - `g++` (GCC compiler)
  - `make`
  - `debhelper` (>= 12) for packaging
- **Windows**: 
  - MinGW-w64 (for GCC on Windows)
  - MSYS2 (optional, for a Unix-like environment)
  - CMake (optional, for build configuration)

## Compilation Instructions

### Linux

1. **Install Dependencies**:
   ```bash
   sudo apt update
   sudo apt install -y g++ make libsdl2-dev libsdl2-mixer-dev libsdl2-image-dev
   ```

2. **Clone the Repository**:
   ```bash
   git clone https://github.com/Gerald1973/BreakWall.git
   cd BreakWall
   ```

3. **Compile the Game**:
   ```bash
   make
   ```

4. **Run the Game**:
   ```bash
   ./breakwall
   ```

### Windows

1. **Install MinGW-w64**:
   - Download and install MinGW-w64 from <https://www.mingw-w64.org/>.
   - Ensure `g++`, `mingw32-make`, and necessary libraries are in your PATH.
   - **Note**: MinGW-w64 provides `mingw32-make.exe` instead of `make.exe` in its `bin` folder. Use `mingw32-make` instead of `make`, or create an alias (e.g., `alias make=mingw32-make` in your shell configuration) to use `make` commands seamlessly.

2. **Install SDL2 Libraries**:
   - Download SDL2, SDL2_mixer, and SDL2_image development libraries for MinGW from the links above.
   - Extract the libraries to a directory, e.g., `C:\SDL2`, `C:\SDL2_mixer`, `C:\SDL2_image`.
   - Ensure the `include` and `lib` directories are accessible to the compiler.

3. **Clone the Repository**:
   - Use Git for Windows or clone via:
     ```bash
     git clone https://github.com/Gerald1973/BreakWall.git
     cd BreakWall
     ```

4. **Compile the Game**:
   - Open a MinGW terminal or MSYS2 shell.
   - Update the Makefile if necessary to point to your SDL2 library paths:
     ```makefile
     INCLUDES = -I"C:/SDL2/include" -I"C:/SDL2_mixer/include" -I"C:/SDL2_image/include"
     LIBS = -L"C:/SDL2/lib" -L"C:/SDL2_mixer/lib" -L"C:/SDL2_image/lib" -lmingw32 -lSDL2main -lSDL2 -lSDL2_mixer -lSDL2_image
     ```
   - Run:
     ```bash
     mingw32-make
     ```
     Alternatively, if you’ve set up the alias, use:
     ```bash
     make
     ```

5. **Run the Game**:
   - Copy required DLLs (e.g., `SDL2.dll`, `SDL2_mixer.dll`, `SDL2_image.dll`) from the SDL2 library `bin` directories to the same directory as the `breakwall.exe`.
   - Run:
     ```bash
     ./breakwall.exe
     ```

## Packaging Instructions

### Linux (Debian Package)

1. **Install Packaging Tools**:
   ```bash
   sudo apt install -y debhelper devscripts build-essential
   ```

2. **Build the Debian Package**:
   - Ensure you are in the project root directory (`BreakWall`).
   - Run:
     ```bash
     dpkg-buildpackage -us -uc
     ```
   - This generates `.deb` files in the parent directory, including `breakwall_1.0_amd64.deb`.

3. **Install the Package**:
   ```bash
   sudo dpkg -i ../breakwall_1.0_amd64.deb
   sudo apt-get install -f  # Resolve dependencies if needed
   ```

4. **Run the Installed Game**:
   ```bash
   breakwall
   ```

### Windows (Manual Packaging)

1. **Create a Distribution Folder**:
   - Create a folder, e.g., `BreakWall_Release`.
   - Copy the compiled `breakwall.exe` to this folder.

2. **Include Dependencies**:
   - Copy the required DLLs (`SDL2.dll`, `SDL2_mixer.dll`, `SDL2_image.dll`, and others listed in `windows/dll/`) to the same folder as `breakwall.exe`.
   - Copy the `resources/` directory (containing `images/`, `mods/`, and `sound/`) to the release folder.

3. **Test the Package**:
   - Run `breakwall.exe` from the release folder to ensure all dependencies are included.

4. **Optional: Create a ZIP Archive**:
   ```bash
   zip -r BreakWall_Release.zip BreakWall_Release/
   ```

## Elements Description

### Screen

The screen resolution is 50% of a Full HD display:
- **Height**: 540 pixels
- **Width**: 960 pixels

### Brick

The standard brick size is based on the texture dimensions:
- **Width**: 64 pixels
- **Height**: 32 pixels

### Wall

The wall is composed of bricks:
- **Maximum Width**: 15 bricks
- **Maximum Height**: 20 bricks

### 7-Segment Display

Used for score display:
- **Width**: 48 pixels
- **Height**: 64 pixels

### Songs

Music files are sourced from the Commodore Amiga era and played using SDL2_mixer.

### Images

- **Preferred Format**: PNG (supports transparent colors).

### Sound

- **Preferred Format**: WAV (chosen for compatibility and quality).

## Notes

- Ensure all resource files (images, sounds, and music) are present in the `resources/` directory as specified in the project structure.
- For licensing concerns, particularly regarding music files, refer to the `debian/copyright` file. Replace unlicensed music with royalty-free alternatives if needed.
- The game is distributed under the MIT License, as specified in the `debian/copyright` file.

For further details, refer to the project repository:  
<https://github.com/Gerald1973/BreakWall>