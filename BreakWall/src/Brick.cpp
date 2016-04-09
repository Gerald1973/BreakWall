#include "Brick.h"
#include "SDL2/SDL.h"

Brick::Brick() {
	this->value = 20;
	this->sound = NULL;
	this->destroyed = false;
	this->textureWithPosition = NULL;
}
void Brick::setValue(int value) {
	this->value = value;
}

int Brick::getValue() {
	return this->value;

}

TextureWithPosition* Brick::getTextureWithPosition() {
	return this->textureWithPosition;
}

void Brick::setTextureWithPosition(TextureWithPosition* textureWithPosition) {
	this->textureWithPosition = textureWithPosition;

}

Brick::~Brick() {
	this->setDestroyed(false);
	delete this->textureWithPosition;
}

bool Brick::isDestroyed() {
	return this->destroyed;
}

void Brick::setDestroyed(bool destroyed) {
	this->destroyed = destroyed;
}

void Brick::render(SDL_Renderer* renderer) {
	if (!isDestroyed()) {
		SDL_RenderCopy(renderer, getTextureWithPosition()->getTexture(), NULL,
				&(getTextureWithPosition()->getPosition()));
	}
}

Mix_Chunk* Brick::getSound() {
	return this->sound;
}

void Brick::setSound(Mix_Chunk* sound) {
	this->sound = sound;
}

