#include <SDL.h>
#include <SDL_image.h>
#include <stdio.h>
#include <string>

#include "Sprite.hpp"

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

	Sprite shroom;
	shroom.LoadTexture("content\\shroom.png", m_gRenderer);

	bool running = true;
	while (running) {

		SDL_Event event;
		while (SDL_PollEvent(&event)) {
		}
		//Clear screen
		SDL_RenderClear(m_gRenderer);
		shroom.Draw({ 0 }, m_gRenderer);
		SDL_RenderPresent(m_gRenderer);
	}

	SDL_DestroyWindow(window);
	return 1;
}