#include <SDL.h>
#include <SDL_image.h>
#include <stdio.h>
#include <string>
#include <vector>

#include "Sprite.hpp"
#include "AnimatedSprite.h"

#undef main

//SCREEN WIDTH / HEIGHT
const int SCREEN_WIDTH = 800;
const int SCREEN_HEIGHT = 400;
int main(int argc, char* argv) {
	if (SDL_Init(SDL_INIT_EVERYTHING) < 0)
	{
		printf("Could not initialize SDL! SDL_Error: %s\n", SDL_GetError());
	}

	SDL_Window* window = nullptr;
	SDL_Renderer* m_gRenderer = nullptr;

	window = SDL_CreateWindow("SDL Tutorial", 50, 50, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN);
	m_gRenderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);

	//Initialize renderer color
	SDL_SetRenderDrawColor(m_gRenderer, 0xFF, 0xFF, 0xFF, 0xFF);

	//Initialize PNG loading
	int imgFlags = IMG_INIT_PNG;
	if (!(IMG_Init(imgFlags) & imgFlags))
	{
		printf("SDL_image could not initialize! SDL_image Error: %s\n", IMG_GetError());
	}

	if (window == nullptr)
	{
		printf("Failed to create window! SDL_Error: %s\n", SDL_GetError());
		return 0;
	}



	Sprite shroom_sprite01;
	shroom_sprite01.LoadTexture("content\\mush\\walk\\move_0.png", m_gRenderer);

	Sprite shroom_sprite02;
	shroom_sprite02.LoadTexture("content\\mush\\walk\\move_1.png", m_gRenderer);

	Sprite shroom_sprite03;
	shroom_sprite03.LoadTexture("content\\mush\\walk\\move_2.png", m_gRenderer);

	Sprite shroom_sprite04;
	shroom_sprite04.LoadTexture("content\\mush\\walk\\move_3.png", m_gRenderer);

	AnimatedSprite shroom;
	shroom.AddSprite(shroom_sprite01);
	shroom.AddSprite(shroom_sprite02);
	shroom.AddSprite(shroom_sprite03);
	shroom.AddSprite(shroom_sprite04);
	shroom.delta = 0.005f;


	bool running = true;
	Uint32 tick = 0;
	while (running) {

		SDL_Event event;
		while (SDL_PollEvent(&event)) {
		}
		//Clear screen
		SDL_RenderClear(m_gRenderer);
		shroom.Animate({ 0 }, m_gRenderer);
		SDL_RenderPresent(m_gRenderer);

		unsigned int fps = 60;
		if ((1000 / fps) > SDL_GetTicks() - tick) {
			SDL_Delay(1000 / fps - (SDL_GetTicks() - tick));
		}
	}

	SDL_DestroyWindow(window);
	return 1;
}