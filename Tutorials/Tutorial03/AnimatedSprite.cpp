#include <SDL.h>
#include <SDL_image.h>
#include <vector>
#include <stdio.h>
#include <string>
#include "Sprite.hpp"
#include "AnimatedSprite.h"

AnimatedSprite::AnimatedSprite()
{
}


AnimatedSprite::~AnimatedSprite()
{
}

void AnimatedSprite::AddSprite(Sprite sprite)
{
	this->animFrames.insert(this->animFrames.end(), sprite);
	this->maxFrames += 1.0f;
}

void AnimatedSprite::Step()
{
	this->frame += this->delta;

	if (this->frame >= this->maxFrames-1) {
		this->frame = 0.0f;
	}
}

void AnimatedSprite::Animate(SDL_Rect pos, SDL_Renderer* gRenderer)
{
	this->Step();
	this->animFrames[static_cast<int>(this->frame)].Draw(pos, gRenderer);
}
