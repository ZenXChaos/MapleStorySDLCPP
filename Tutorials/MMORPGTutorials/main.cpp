#include <SDL.h>
#include <stdio.h>

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

	window = SDL_CreateWindow("SDL Tutorial", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN);
	m_gRenderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);

	//Initialize renderer color
	SDL_SetRenderDrawColor(m_gRenderer, 0xFF, 0xFF, 0xFF, 0xFF);

	if (window == nullptr)
	{
		printf("Failed to create window! SDL_Error: %s\n", SDL_GetError());
		return 0;
	}

	bool running = true;
	while (running) {

		SDL_Event event;
		while (SDL_PollEvent(&event)) {

		}
	}

	return 1;
}