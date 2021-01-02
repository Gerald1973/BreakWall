#include "../include/Wall.hpp"

#include <SDL2/SDL.h>

#include "../include/Brick.h"
#include "../include/GlobalConstants.h"
#include "../include/UtilConstants.h"
#include "../include/InitUtils.hpp"
#include "../include/CustomEventUtils.hpp"
#include <iostream>

Wall::Wall() {
}

Wall::~Wall() {
}

void Wall::build() {
}

void Wall::render() {
	for (unsigned int i = 0; i < bricks.size(); i++) {
		bricks[i]->render();
	}
}

std::vector<Brick*> Wall::getBricks() {
	return this->bricks;
}

void Wall::setBricks(std::vector<Brick*> bricks) {
	this->bricks = bricks;
}

void Wall::performEvent(SDL_Event &event) {
	if (event.user.code == CustomEventUtils::Code::BALL_MOVED) {
		for (unsigned int i = 0; i < this->bricks.size(); i++) {
			Ball *ball = (Ball*) event.user.data1;
			if (this->bricks[i]->isTouchedByBall(ball)) {
				this->bricks[i]->performEvent(event);
				break;
			}
		}
	} else if (event.user.code == CustomEventUtils::Code::BRICK_REMOVED) {
		Brick *brick = (Brick*) event.user.data1;
		int index = this->findIndex(brick);
		this->bricks.erase(bricks.begin() + index);
		delete brick;
		CustomEventUtils::getInstance()->postEventBrickRemaining(this);
	}
}

int Wall::findIndex(Brick *brick) {
	int result = -1;
	if (this->bricks.size() > 0) {
		int size = (int) bricks.size();
		for (int i = 0; i < size; i++) {
			if (bricks[i] == brick) {
				result = i;
				break;
			}
		}
	}
	return result;
}

void Wall::init() {
	std::cout << "DEBUG: init wall" << std::endl;
	std::vector<Brick*> bricks;
	for (int y = 0; y < GlobalConstants::MAX_NUMBER_OF_BRICKS_ON_Y; y = y + 3) {
		int x = y % 2;
		for (; x < GlobalConstants::MAX_NUMBER_OF_BRICKS_ON_X; x = x + 5) {
			Brick *brick = new Brick();
			brick->init();
			brick->getTextureWithPosition()->setX(x * brick->getTextureWithPosition()->getPosition().w);
			brick->getTextureWithPosition()->setY(y * brick->getTextureWithPosition()->getPosition().h);
			bricks.push_back(brick);
		}
	}
	setBricks(bricks);
	build();
}

std::string Wall::getSongFileName() {
	return this->songFileName;
}

std::string Wall::getSongKey() {
	return this->songKey;
}

void Wall::setSongFileName(std::string songFileName) {
	this->songFileName = songFileName;
}

void Wall::setSongKey(std::string songKey) {
	this->songKey = songKey;
}

void Wall::playSong() {
//	Uint8 *data = nullptr;
//	Uint32 len = 64 * 1024;
//	if(SDL_GetQueuedAudioSize(sdlAudioDeviceId) == 0){
//		printStatus(sdlAudioDeviceId);
//		std::cout << "Sample remaining : " << microModSDLPlayer->getSamplesRemaining() << std::endl;
//		data = microModSDLPlayer->buildStream(len);
//		if (data != nullptr){
//			SDL_QueueAudio(this->sdlAudioDeviceId, data, len);
//			free(data);
//		}
//	}
}


void Wall::printStatus(SDL_AudioDeviceID dev)
{
    switch (SDL_GetAudioDeviceStatus(dev))
    {
        case SDL_AUDIO_STOPPED: printf("stopped\n"); break;
        case SDL_AUDIO_PLAYING: printf("playing\n"); break;
        case SDL_AUDIO_PAUSED: printf("paused\n"); break;
        default: printf("???"); break;
    }
}

SDL_AudioDeviceID Wall::initSong() {
this->setSongFileName("worldofw.mod");
this->setSongKey("level_001");
Mix_Music *mixMusic = Mix_LoadMUS(this->getSongFileName().c_str());
Mix_PlayMusic(mixMusic,1);
//InitUtils::getInstance()->addMod(this->getSongFileName(), this->getSongKey());
//std::vector<unsigned char> modFile = InitUtils::getInstance()->getMapMods()[this->getSongKey()];
//std::cout << "Mod file size : " << modFile.size() << std::endl;
//sdlAudioDeviceId =microModSDLPlayer->initialise(&modFile[0]);
//Uint8 *data = nullptr;
//Uint32 len = 64 * 1024;
//Uint32 remainingByte = SDL_GetQueuedAudioSize(this->sdlAudioDeviceId);
//
//
//
//SDL_PauseAudioDevice(this->sdlAudioDeviceId, 0);
//data =microModSDLPlayer->buildStream(len - remainingByte);
//SDL_QueueAudio(this->sdlAudioDeviceId, data, len);
//free(data);
//data =microModSDLPlayer->buildStream(len - remainingByte);
//SDL_QueueAudio(this->sdlAudioDeviceId, data, len);
//free(data);
//data =microModSDLPlayer->buildStream(len - remainingByte);
//SDL_QueueAudio(this->sdlAudioDeviceId, data, len);
//free(data);
//data =microModSDLPlayer->buildStream(len - remainingByte);
//SDL_QueueAudio(this->sdlAudioDeviceId, data, len);
//free(data);
//data =microModSDLPlayer->buildStream(len - remainingByte);
//SDL_QueueAudio(this->sdlAudioDeviceId, data, len);
//free(data);
//data =microModSDLPlayer->buildStream(len - remainingByte);
//SDL_QueueAudio(this->sdlAudioDeviceId, data, len);
//free(data);
//data =microModSDLPlayer->buildStream(len - remainingByte);
//SDL_QueueAudio(this->sdlAudioDeviceId, data, len);
//free(data);
//data =microModSDLPlayer->buildStream(len - remainingByte);
//SDL_QueueAudio(this->sdlAudioDeviceId, data, len);
//free(data);
//data =microModSDLPlayer->buildStream(len - remainingByte);
//SDL_QueueAudio(this->sdlAudioDeviceId, data, len);
//free(data);
//data =microModSDLPlayer->buildStream(len - remainingByte);
//SDL_QueueAudio(this->sdlAudioDeviceId, data, len);
//free(data);
//data =microModSDLPlayer->buildStream(len - remainingByte);
//SDL_QueueAudio(this->sdlAudioDeviceId, data, len);
//free(data);
//data =microModSDLPlayer->buildStream(len - remainingByte);
//SDL_QueueAudio(this->sdlAudioDeviceId, data, len);
//free(data);
//data =microModSDLPlayer->buildStream(len - remainingByte);
//SDL_QueueAudio(this->sdlAudioDeviceId, data, len);
//free(data);
//data =microModSDLPlayer->buildStream(len - remainingByte);
//SDL_QueueAudio(this->sdlAudioDeviceId, data, len);
//free(data);
//data =microModSDLPlayer->buildStream(len - remainingByte);
//SDL_QueueAudio(this->sdlAudioDeviceId, data, len);
//free(data);
//data =microModSDLPlayer->buildStream(len - remainingByte);
//SDL_QueueAudio(this->sdlAudioDeviceId, data, len);
//free(data);
//data =microModSDLPlayer->buildStream(len - remainingByte);
//SDL_QueueAudio(this->sdlAudioDeviceId, data, len);
//free(data);
//data =microModSDLPlayer->buildStream(len - remainingByte);
//SDL_QueueAudio(this->sdlAudioDeviceId, data, len);
//free(data);
//data =microModSDLPlayer->buildStream(len - remainingByte);
//SDL_QueueAudio(this->sdlAudioDeviceId, data, len);
//free(data);
//data =microModSDLPlayer->buildStream(len - remainingByte);
//SDL_QueueAudio(this->sdlAudioDeviceId, data, len);
//free(data);
//data =microModSDLPlayer->buildStream(len - remainingByte);
//SDL_QueueAudio(this->sdlAudioDeviceId, data, len);
//free(data);
//data =microModSDLPlayer->buildStream(len - remainingByte);
//SDL_QueueAudio(this->sdlAudioDeviceId, data, len);
//free(data);
//data =microModSDLPlayer->buildStream(len - remainingByte);
//SDL_QueueAudio(this->sdlAudioDeviceId, data, len);
//free(data);
//data =microModSDLPlayer->buildStream(len - remainingByte);
//SDL_QueueAudio(this->sdlAudioDeviceId, data, len);
//free(data);
//data =microModSDLPlayer->buildStream(len - remainingByte);
//SDL_QueueAudio(this->sdlAudioDeviceId, data, len);
//free(data);
//data =microModSDLPlayer->buildStream(len - remainingByte);
//SDL_QueueAudio(this->sdlAudioDeviceId, data, len);
//free(data);
//data =microModSDLPlayer->buildStream(len - remainingByte);
//SDL_QueueAudio(this->sdlAudioDeviceId, data, len);
//free(data);
//data =microModSDLPlayer->buildStream(len - remainingByte);
//SDL_QueueAudio(this->sdlAudioDeviceId, data, len);
//free(data);
//data =microModSDLPlayer->buildStream(len - remainingByte);
//SDL_QueueAudio(this->sdlAudioDeviceId, data, len);
//free(data);
//data =microModSDLPlayer->buildStream(len - remainingByte);
//SDL_QueueAudio(this->sdlAudioDeviceId, data, len);
//free(data);
//data =microModSDLPlayer->buildStream(len - remainingByte);
//SDL_QueueAudio(this->sdlAudioDeviceId, data, len);
//free(data);
//data =microModSDLPlayer->buildStream(len - remainingByte);
//SDL_QueueAudio(this->sdlAudioDeviceId, data, len);
//free(data);
//data =microModSDLPlayer->buildStream(len - remainingByte);
//SDL_QueueAudio(this->sdlAudioDeviceId, data, len);
//free(data);
//data =microModSDLPlayer->buildStream(len - remainingByte);
//SDL_QueueAudio(this->sdlAudioDeviceId, data, len);
//free(data);
//data =microModSDLPlayer->buildStream(len - remainingByte);
//SDL_QueueAudio(this->sdlAudioDeviceId, data, len);
//free(data);
//data =microModSDLPlayer->buildStream(len - remainingByte);
//SDL_QueueAudio(this->sdlAudioDeviceId, data, len);
//free(data);
//data =microModSDLPlayer->buildStream(len - remainingByte);
//SDL_QueueAudio(this->sdlAudioDeviceId, data, len);
//free(data);
//data =microModSDLPlayer->buildStream(len - remainingByte);
//SDL_QueueAudio(this->sdlAudioDeviceId, data, len);
//free(data);
//SDL_QueueAudio(this->sdlAudioDeviceId, data, len);
//free(data);
//data =microModSDLPlayer->buildStream(len - remainingByte);
//SDL_QueueAudio(this->sdlAudioDeviceId, data, len);
//free(data);
//data =microModSDLPlayer->buildStream(len - remainingByte);
//SDL_QueueAudio(this->sdlAudioDeviceId, data, len);
//free(data);
//data =microModSDLPlayer->buildStream(len - remainingByte);
//SDL_QueueAudio(this->sdlAudioDeviceId, data, len);
//free(data);
//data =microModSDLPlayer->buildStream(len - remainingByte);
//SDL_QueueAudio(this->sdlAudioDeviceId, data, len);
//free(data);
//data =microModSDLPlayer->buildStream(len - remainingByte);
//SDL_QueueAudio(this->sdlAudioDeviceId, data, len);
//free(data);
//std::cout << "SDL_GetQueuedAudioSize 0 =" << SDL_GetQueuedAudioSize(this->sdlAudioDeviceId) << std::endl;
//std::cout << "SDL_GetQueuedAudioSize  1=" << SDL_GetQueuedAudioSize(this->sdlAudioDeviceId) << std::endl;
//return sdlAudioDeviceId;
}

SDL_AudioDeviceID Wall::getSdlAudioDeviceId() {
//return sdlAudioDeviceId;
}

void Wall::stopSong() {
//SDL_PauseAudioDevice(sdlAudioDeviceId, 1);
//SDL_ClearQueuedAudio(sdlAudioDeviceId);
}
