
#ifdef __cplusplus
#include <cstdlib>
#else
#include <stdlib.h>
#endif

#include <SDL2/SDL.h>
#include <iostream>
#include <map>
#include <string>
#include "../include/UtilConstants.h"
#include "../include/Brick.h"
#include "../include/Bare.h"
#include "../include/Wall.hpp"
#include "../include/Ball.h"
#include "../include/ScoreSegments.hpp"
#include <vector>
#include <SDL2/SDL_mixer.h>
#include <SDL2/SDL_image.h>
#include "../include/Background.hpp"
#include "../include/GlobalConstants.h"
#include "../include/FileUtils.hpp"
#include "../include/MicroModUtils.hpp"
#include "../include/MicroModSDLPlayer.hpp"
#include "../include/CustomEventUtils.hpp"
#include "../include/Title.hpp"

Brick *wall[20][30];
int dirX = 1;
int dirY = 1;
bool success = true;
std::map<std::string, Mix_Chunk *> mapSounds;
std::map<std::string, SDL_Texture *> mapTextures;

void displayErrorMessage()
{
    std::cout << SDL_GetError() << "\n";
}

SDL_Texture *loadTexture(std::string fileName, SDL_Renderer *renderer)
{
    SDL_Texture *result = NULL;
    result = IMG_LoadTexture(renderer, fileName.c_str());
    if (result == NULL)
    {
        std::cout << "Impossible to load : " << fileName << " error:" << SDL_GetError() << std::endl;
    }
    return result;
}

void initTextureMap(SDL_Renderer *renderer)
{
    mapTextures["ball"] = loadTexture("ball.png", renderer);
    mapTextures["bare"] = loadTexture("bare.png", renderer);
    mapTextures["brickRed"] = loadTexture("brickRed.bmp", renderer);
    mapTextures["segments"] = loadTexture("segments_48_64.png", renderer);
    mapTextures["background001"] = loadTexture("winter_01.jpg", renderer);
    mapTextures["title"]=loadTexture("breakwall_title.png", renderer);
}

Mix_Chunk *loadSoundEffect(const char *fileName)
{
    Mix_Chunk *result = NULL;
    result = Mix_LoadWAV(fileName);
    if (!result)
    {
        std::cout << "Sound effect error : " << Mix_GetError() << std::endl;
    }
    return result;
}

void initSoundMap()
{
    mapSounds["brick"] = loadSoundEffect("metal.wav");
    mapSounds["bare"] = loadSoundEffect("bare.wav");
}

Bare *initBare(SDL_Renderer *renderer)
{
    Bare *result = new Bare();
    SDL_Rect position;
    TextureWithPosition *textureWithPosition = new TextureWithPosition(mapTextures["bare"], position);
    textureWithPosition->setX((UtilConstants::getInstance()->gameZone.w - textureWithPosition->getPosition().w) / 2);
    textureWithPosition->setY(UtilConstants::getInstance()->gameZone.h - textureWithPosition->getPosition().h);
    result->setTextureWithPosition(textureWithPosition);
    result->setSound(mapSounds["bare"]);
    return result;
}

Wall *initWall(SDL_Renderer *renderer)
{
    Wall *result = new Wall();
    std::vector<std::vector<Brick *>> lines;
    for (int y = 0; y < GlobalConstants::MAX_NUMBER_OF_BRICKS_ON_Y; y++)
    {
        std::vector<Brick *> columns;
        for (int x = 0; x < GlobalConstants::MAX_NUMBER_OF_BRICKS_ON_X; x++)
        {
            Brick *brick = new Brick();
            brick->setSound(mapSounds["brick"]);
            SDL_Rect tmpRect;
            TextureWithPosition *textureWithPosition = new TextureWithPosition(mapTextures["brickRed"], tmpRect);
            brick->setTextureWithPosition(textureWithPosition);
            columns.push_back(brick);
        }
        lines.push_back(columns);
    }
    result->setBricks(lines);
    return result;
}

Ball *initBall(SDL_Renderer *renderer, Wall *wall, Bare *bare)
{
    SDL_Rect rect;
    rect.x = UtilConstants::getInstance()->gameZone.w / 2 + UtilConstants::getInstance()->gameZone.x;
    rect.y = (GlobalConstants::MAX_NUMBER_OF_BRICKS_ON_Y + 1) * GlobalConstants::BRICK_HEIGHT + UtilConstants::getInstance()->gameZone.y;
    TextureWithPosition *textureWithPosition = new TextureWithPosition(mapTextures["ball"], rect);
    Ball *ball = new Ball();
    ball->setTextureWithPosition(textureWithPosition);
    ball->setBare(bare);
    ball->setWall(wall);
    return ball;
}

Background *initBackground(SDL_Renderer *renderer)
{
    Background *result = new Background();
    TextureWithPosition *textureWithPosition = new TextureWithPosition(
        mapTextures["background001"],
        UtilConstants::getInstance()->gameZone.x,
        UtilConstants::getInstance()->gameZone.y,
        UtilConstants::getInstance()->gameZone.w,
        UtilConstants::getInstance()->gameZone.h);
    result->setTextureWithPosition(textureWithPosition);
    return result;
}

Title *initTitle(SDL_Renderer *renderer)
{
	Title *result = new Title();
    TextureWithPosition *textureWithPosition = new TextureWithPosition(
        mapTextures["title"],
        0,
        0,
		GlobalConstants::SCREEN_WIDTH,
        GlobalConstants::SCREEN_HEIGHT);
    result->setTextureWithPosition(textureWithPosition);
    return result;
}

ScoreSegments *initScoreSegments(SDL_Renderer *renderer)
{
    ScoreSegments *result = new ScoreSegments();
    result->setTexture(mapTextures["segments"]);
    return result;
}

void quit()
{
    std::map<std::string, Mix_Chunk *>::iterator mapSoundsIterator;
    for (mapSoundsIterator = mapSounds.begin(); mapSoundsIterator != mapSounds.end(); mapSoundsIterator++)
    {
        std::cout << "Remove :" << mapSoundsIterator->first << "\n";
        Mix_FreeChunk(mapSoundsIterator->second);
    }
    std::map<std::string, SDL_Texture *>::iterator mapTexturesIterator;
    for (mapTexturesIterator = mapTextures.begin(); mapTexturesIterator != mapTextures.end(); mapTexturesIterator++)
    {
        std::cout << "Remove :" << mapTexturesIterator->first << "\n";
        SDL_DestroyTexture(mapTexturesIterator->second);
    }
    Mix_Quit();
    IMG_Quit();
    SDL_Quit();
}

int main(int argc, char **argv)
{
    // initialize SDL video
    if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO) < 0)
    {
        std::cout << "Unable to init SDL: " << SDL_GetError() << "\n";
        return 1;
    }
    if (Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 2048) < 0)
    {
        std::cout << "SDL_mixer could not initialize! SDL_mixer Error: " << Mix_GetError() << "\n";
    }

    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 2);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 1);
    SDL_SetRelativeMouseMode(SDL_TRUE);
    const Uint8 *keys = SDL_GetKeyboardState(NULL);
    std::map<int, bool> mapKbd;
    SDL_Window *pWindow = SDL_CreateWindow("BreakWall", 0, 0, GlobalConstants::SCREEN_WIDTH, GlobalConstants::SCREEN_HEIGHT, SDL_WINDOW_OPENGL | SDL_WINDOW_OPENGL | SDL_WINDOW_RESIZABLE);
    if (pWindow == NULL)
    {
        std::cout << SDL_GetError() << "\n";
    }
    SDL_Renderer *renderer = SDL_CreateRenderer(pWindow, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_TARGETTEXTURE | SDL_RENDERER_PRESENTVSYNC);
    SDL_Event event;
    SDL_Texture *tmpTexture = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_ABGR8888, SDL_TEXTUREACCESS_TARGET, GlobalConstants::SCREEN_WIDTH, GlobalConstants::SCREEN_HEIGHT);
    initSoundMap();
    initTextureMap(renderer);
    Bare *bare = initBare(renderer);
    Wall *wall = initWall(renderer);
    wall->build();
    ScoreSegments *scoreSegments = initScoreSegments(renderer);
    Ball *ball = initBall(renderer, wall, bare);
    Background *background = initBackground(renderer);
    Title *title = initTitle(renderer);
    bool loop = true;
    //Test amiga mod
    std::string song = "worldofw.mod";
    std::vector<unsigned char> moduleHeader = FileUtils::getInstance()->readFile(song,0,1084);
    std::vector<unsigned char> mf = FileUtils::getInstance()->readFile(song);
    std::cout << "DEBUG : file size          :" << mf.size() << std::endl;
    std::cout << "DEBUG : readFromTO         :" << moduleHeader.size() << std::endl;
    std::cout << "DEBUG : micromod version   :" << MicroModUtils::getInstance()->getVersion() << std::endl;
    std::cout << "DEBUG : number of channel  :" << MicroModUtils::getInstance()->calculateNumChannels(&moduleHeader[0]) << std::endl;
    std::cout << "DEBUG : number of patterns :" << MicroModUtils::getInstance()->calculateNumPatterns(&moduleHeader[0]) << std::endl;
    MicroModSDLPlayer::getInstance()->initialise(&mf[0]);
    SDL_PauseAudio(0);
    //End test
    while (loop)
    {
        SDL_SetRenderTarget(renderer, tmpTexture);
        SDL_RenderClear(renderer);
        ball->moveBall();
        while (SDL_PollEvent(&event))
        {
            switch (event.type)
            {
            case SDL_KEYUP:
                mapKbd[event.key.keysym.scancode] = false;
                break;
            case SDL_KEYDOWN:
                keys = SDL_GetKeyboardState(NULL);
                break;
            case SDL_MOUSEMOTION:
                bare->getTextureWithPosition()->setX(bare->getTextureWithPosition()->getX() + event.motion.xrel);
                break;
            case SDL_USEREVENT:
                if (event.user.code == CustomEventUtils::Code::SONG_STOP) {
                    SDL_PauseAudio(1);
                    MicroModSDLPlayer::getInstance()->initialise(&mf[0]);
                    SDL_PauseAudio(0);
                } else if (event.user.code == CustomEventUtils::Code::BRICK_TOUCHED){
                    Brick* brick = (Brick*) event.user.data1;
                    scoreSegments->addScore(brick->getValue());
                } else if (event.user.code == CustomEventUtils::Code::BORDER_BOTTOM_TOUCHED){
                    std::cout << "Todo one live less." << std::endl;
                }
                break;
            }
        }
        if (keys[SDL_SCANCODE_ESCAPE])
        {
            loop = false;
        }
        background->render(renderer);
        wall->render(renderer);
        bare->render(renderer);
        ball->render(renderer);
        title->render(renderer);
        scoreSegments->render(renderer);

        SDL_SetRenderTarget(renderer, NULL);
        SDL_RenderCopy(renderer, tmpTexture, NULL, NULL);
        SDL_RenderPresent(renderer);
    }
    SDL_DestroyWindow(pWindow);
    quit();
    return 0;
}
