#ifdef __cplusplus
    #include <cstdlib>
#else
    #include <stdlib.h>
#endif

#include <SDL2/SDL.h>
//#include <SDL2/SDL_opengles2.h>
#include <iostream>
#include <map>
#include "UtilConstants.h"
#include "Brick.h"
#include "Bare.h"
#include "Wall.h"
#include "Ball.h"
#include "ScoreSegments.h"
#include <vector>
#include <SDL2/SDL_mixer.h>
#include <SDL2/SDL_image.h>


Brick* wall[20][30];
int dirX = 1;
int dirY = 1;
bool success = true;
std::map<std::string,Mix_Chunk*> mapSounds;
std::map<std::string,SDL_Texture*> mapTextures;

void displayErrorMessage(){
    std::cout << SDL_GetError() << "\n";
}

SDL_Texture* loadTexture(const char *fileName,SDL_Renderer* renderer){
    SDL_Texture* result = NULL;
    result = IMG_LoadTexture(renderer,fileName);
    if (result == NULL){
    	std::cout << "Impossible to load : " << *fileName <<  " error:" << SDL_GetError() << std::endl;
    }
    return result;
}



void initTextureMap(SDL_Renderer *renderer){
    mapTextures["ball"] = loadTexture("ball.png",renderer);
    mapTextures["bare"] = loadTexture("bare.png",renderer);
    mapTextures["brickRed"] = loadTexture("brickRed.bmp",renderer);
    mapTextures["segments"] = loadTexture("segments.png",renderer);
}

Mix_Chunk* loadSoundEffect(const char *fileName){
	Mix_Chunk* result = NULL;
	result = Mix_LoadWAV(fileName);
	if (!result){
		std::cout << "Sound effect error : " << Mix_GetError() << std::endl;
	}
	return result;
}

void initSoundMap(){
	mapSounds["brick"]=loadSoundEffect("metal.wav");
	mapSounds["bare"]=loadSoundEffect("bare.wav");
}

Bare* initBare(SDL_Renderer *renderer){
    Bare* result = new Bare();
    SDL_Rect position;
    TextureWithPosition *textureWithPosition = new TextureWithPosition(mapTextures["bare"],position);
    textureWithPosition->setX(UtilConstants::getInstance()->screenSize.w / 2 -textureWithPosition->getPosition().w/2);
    textureWithPosition->setY(UtilConstants::getInstance()->screenSize.h - textureWithPosition->getPosition().h);
    result->setTextureWithPosition(textureWithPosition);
    result->setSound(mapSounds["bare"]);
    return result;
}

Wall* initWall(SDL_Renderer *renderer){
    Wall *result = new Wall();
    std::vector <std::vector<Brick*> > lines;
    for (int y = 0; y < GlobalConstants::MAX_NUMBER_OF_BRICKS_ON_Y;y++){
        std::vector<Brick*> columns;
        for (int x = 0; x < GlobalConstants::MAX_NUMBER_OF_BRICKS_ON_X;x++){
            Brick *brick = new Brick();
            brick->setSound(mapSounds["brick"]);
            SDL_Rect tmpRect;
            TextureWithPosition * textureWithPosition = new TextureWithPosition(mapTextures["brickRed"],tmpRect);
            brick->setTextureWithPosition(textureWithPosition);
            columns.push_back(brick);
        }
        lines.push_back(columns);
    }
    result->setBricks(lines);
    return result;
}

Ball* initBall(SDL_Renderer *renderer, Wall* wall,Bare *bare){
    SDL_Rect rect;
    rect.x = UtilConstants::getInstance()->screenSize.w / 2 + UtilConstants::getInstance()->screenSize.x;
    rect.y = UtilConstants::getInstance()->screenSize.h / 2 + UtilConstants::getInstance()->screenSize.y;
    TextureWithPosition *textureWithPosition = new TextureWithPosition(mapTextures["ball"],rect);
    Ball *ball = new Ball();
    ball->setTextureWithPosition(textureWithPosition);
    ball->setBare(bare);
    ball->setWall(wall);
    return ball;
}

ScoreSegments* initScoreSegments(SDL_Renderer *renderer){
	ScoreSegments* result =new ScoreSegments();
	result->setTexture(mapTextures["segments"]);
	return result;
}

void quit(){
    std::map<std::string,Mix_Chunk*>::iterator  mapSoundsIterator;
    for (mapSoundsIterator = mapSounds.begin(); mapSoundsIterator != mapSounds.end(); mapSoundsIterator++){
        std::cout << "Remove :" << mapSoundsIterator->first << "\n";
        Mix_FreeChunk(mapSoundsIterator->second);
    }
    std::map<std::string,SDL_Texture*>::iterator  mapTexturesIterator;
    for (mapTexturesIterator = mapTextures.begin(); mapTexturesIterator != mapTextures.end(); mapTexturesIterator++){
        std::cout << "Remove :" << mapTexturesIterator->first << "\n";
        SDL_DestroyTexture(mapTexturesIterator->second);
    }
    Mix_Quit();
    IMG_Quit();
    SDL_Quit();
}

int main ( int argc, char** argv )
{
    // initialize SDL video
    if ( SDL_Init( SDL_INIT_VIDEO|SDL_INIT_AUDIO) < 0 )
    {
        std::cout << "Unable to init SDL: " << SDL_GetError() << "\n";
        return 1;
    }
    if( Mix_OpenAudio( 44100, MIX_DEFAULT_FORMAT, 2, 2048 ) < 0 ){
        std::cout << "SDL_mixer could not initialize! SDL_mixer Error: " << Mix_GetError() << "\n";
    }


    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 2);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 1);
    //SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_ES);
    const Uint8* keys = SDL_GetKeyboardState(NULL);
    std::map<int,bool> mapKbd;
    SDL_Window *pWindow = SDL_CreateWindow("",UtilConstants::getInstance()->screenSize.x,UtilConstants::getInstance()->screenSize.y,UtilConstants::getInstance()->screenSize.w,UtilConstants::getInstance()->screenSize.h,
    		SDL_WINDOW_OPENGL|SDL_WINDOW_OPENGL|SDL_WINDOW_RESIZABLE);
    		//SDL_WINDOW_FULLSCREEN|SDL_WINDOW_OPENGL);
    if (pWindow == NULL){
        std::cout <<  SDL_GetError() << "\n";
    }
    SDL_Renderer *renderer =SDL_CreateRenderer(pWindow,-1,SDL_RENDERER_ACCELERATED|SDL_RENDERER_TARGETTEXTURE /*| SDL_RENDERER_PRESENTVSYNC*/);
    SDL_Event event;
    SDL_Texture *tmpTexture = SDL_CreateTexture(renderer,SDL_PIXELFORMAT_ABGR8888,SDL_TEXTUREACCESS_TARGET,UtilConstants::getInstance()->screenSize.w,UtilConstants::getInstance()->screenSize.h);
    initSoundMap();
    initTextureMap(renderer);
    Bare* bare = initBare(renderer);
    Wall* wall = initWall(renderer);
    wall->build();
    ScoreSegments* scoreSegments = initScoreSegments(renderer);
    Ball* ball = initBall(renderer,wall, bare);
    ball->setScoreSegments(scoreSegments);
    bool loop = true;
    while (loop){
        SDL_SetRenderTarget(renderer,tmpTexture);
        SDL_RenderClear(renderer);
        ball->moveBall();
        while (SDL_PollEvent(&event)){
            switch (event.type){
                case SDL_KEYUP:
                    mapKbd[event.key.keysym.scancode] = false;
                    break;
                case SDL_KEYDOWN:
                    keys = SDL_GetKeyboardState(NULL);
                    break;
                case SDL_MOUSEMOTION:
                    bare->getTextureWithPosition()->setX(bare->getTextureWithPosition()->getX()+event.motion.xrel);

            }
        }
        if (keys[SDL_SCANCODE_ESCAPE]){
            loop = false;
        }
        wall->render(renderer);
        bare->render(renderer);
        ball->render(renderer);
        scoreSegments->render(renderer);
        SDL_SetRenderTarget(renderer,NULL);
        SDL_RenderCopy(renderer,tmpTexture,NULL,NULL);
        SDL_RenderPresent(renderer);
    }
    SDL_DestroyWindow(pWindow);
    quit();
    return 0;
}
