#include <SDL.h>
#include <SDL_image.h>
#include <stdio.h>
#include <stdlib.h>
#include <string>
#include <map>
#include <vector>

using namespace std;

#include "AnimatedSprite.h"
#include "RelativeSpace.h"

bool AnimatedSprite::LoadTexture(std::string path, SDL_Renderer* gRenderer)
{
	//The final texture
	SDL_Texture* newTexture = NULL;

	//Load image at specified path
	SDL_Surface* loadedSurface = IMG_Load(path.c_str());
	if (loadedSurface == NULL)
	{
		printf("Unable to load image %s! SDL_image Error: %s\n", path.c_str(), IMG_GetError());
		return false;
	}
	else
	{
		//Create texture from surface pixels
		this->texture = SDL_CreateTextureFromSurface(gRenderer, loadedSurface);
		if (this->texture == NULL)
		{
			printf("Unable to create texture from %s! SDL Error: %s\n", path.c_str(), SDL_GetError());
			return false;
		}

		this->renderer = gRenderer;

		//Get rid of old loaded surface
		SDL_FreeSurface(loadedSurface);
	}

	return true;
}

void AnimatedSprite::Animate(SDL_Rect pos, double angle, SDL_Point* center, SDL_RendererFlip flip, SDL_Rect* frameData = nullptr)
{
	//Set rendering space and render to screen
	SDL_Rect renderQuad = this->animclips[static_cast<int>(this->current_frame)];

	//Set clip rendering dimensions
	if (this->texture != NULL)
	{
		renderQuad.w = this->animclips[static_cast<int>(this->current_frame)].w;
		renderQuad.h = this->animclips[static_cast<int>(this->current_frame)].h;
		renderQuad.x = pos.x + xfactor;
		renderQuad.y = pos.y + yfactor;

		if (frameData != nullptr) {
			*frameData = renderQuad;
		}
	}

	//Render to screen
	SDL_RenderCopyEx(this->renderer, this->texture, &this->animclips[static_cast<int>(this->current_frame)], &renderQuad, angle, center, flip);

	//SDL_RenderCopy(this->renderer, this->texture, &this->animclips[static_cast<int>(this->current_frame)], &pos);
	this->current_frame += this->delta;
	this->animFinished = false;

	if (this->current_frame > this->max_frames - 1) {
		this->current_frame = 0;
		this->animFinished = true;
	}

	this->percentDone = (this->current_frame/this->max_frames)*100;
}

void AnimatedSprite::BuildAnimation(int row, int cnt, int w, int h, float d = 0.1f)
{
	for (int i = 0; i < cnt; i += 1) {
		this->animclips[i].x = 0 + i * w;
		this->animclips[i].y = row * h;
		this->animclips[i].w = w;
		this->animclips[i].h = h;
	}

	this->max_frames = static_cast<float>(cnt) + 0.9f;
	this->delta = d;
}