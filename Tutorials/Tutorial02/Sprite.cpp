#include <SDL.h>
#include <SDL_image.h>
#include <stdio.h>
#include <string>
#include "Sprite.hpp"



bool Sprite::LoadTexture(std::string path, SDL_Renderer* gRenderer)
{

	//Load image
	SDL_Surface* loadedSurface = IMG_Load(path.c_str());
	if (loadedSurface == nullptr)
	{
		printf("Failed to load image %s! SDL_image Error: %s\n", path.c_str(), IMG_GetError());
		return false;
	}
	else
	{
		//Create texture from surface pixels
		this->texture = SDL_CreateTextureFromSurface(gRenderer, loadedSurface);
		if (this->texture == nullptr)
		{
			printf("Unable to create texture from %s! SDL Error: %s\n", path.c_str(), SDL_GetError());
			return false;
		}

		SDL_QueryTexture(this->texture, NULL, NULL, &this->w, &this->h);

		//Free old loaded surface
		SDL_FreeSurface(loadedSurface);
	}

	return true;
}

void Sprite::Draw(SDL_Rect pos, SDL_Renderer* gRenderer)
{
	SDL_Rect newPos = pos;
	newPos.w = this->w;
	newPos.h = this->h;
	SDL_RenderCopy(gRenderer, this->texture, NULL, &newPos);
}

Sprite::Sprite()
{
}


Sprite::~Sprite()
{
}
