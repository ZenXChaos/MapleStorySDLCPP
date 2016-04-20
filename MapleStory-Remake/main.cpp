#include <stdio.h>
#include <string>

#include <tinyxml2.h>

#include <SDL.h>
#include <SDL_image.h>
#include <map>
#include <vector>

using namespace tinyxml2;
using namespace std;

#include "map.h"
#include "player.h"

int main(int argc, char* argv[]) {
	GAME game;
	game.loadMobList();
	//return 1;
	SDL_Init(SDL_INIT_EVERYTHING);

	PLAYER player;

	SDL_Window* window = nullptr;
	SDL_Surface* windowSurface = nullptr;

	window = SDL_CreateWindow("Hi", 50, 50, 679, 376, SDL_WINDOW_RESIZABLE);
	windowSurface = SDL_GetWindowSurface(window);

	Uint32 color = SDL_MapRGB(windowSurface->format, 0xff, 0xff, 0xff);
	Uint32 tick;
	bool running = true;
	float frame = 0.0f;

	std::vector<void*> ent;

	NPC_JAMIE npc_jamie(windowSurface);

	npc_jamie.winSurface = windowSurface;


	MAP home_map;
	
	SDL_FillRect(windowSurface, NULL, SDL_MapRGB(windowSurface->format, 255, 255, 255));
	while (running) {
		//printf("SDL_Init failed: %s\n", SDL_GetError());

		tick = SDL_GetTicks();
		SDL_Event event;

		while (SDL_PollEvent(&event)) {
			switch (event.type) {
			case SDL_QUIT:
				running = false;
				break;
			case SDL_KEYDOWN:

				player.KeyboardInput->keyDownEvent(event, tick);
				

				break;
			case SDL_KEYUP:

				player.KeyboardInput->keyUpEvent(event, tick);
				//player.playerMotorize(event);
				break;
			}

			//
		}
		player.playerMotorize(event);
		frame += 0.1f;

		SDL_FillRect(windowSurface, &windowSurface->clip_rect, color);
		home_map.displayMap(windowSurface);
		game.displayAllMobs(windowSurface, player);
		player.playAnimation(windowSurface);

		npc_jamie.setTarget(player);
		npc_jamie.scanTarget();
		npc_jamie.collider.findCollision(player.playerRect);

		npc_jamie.playAnimation(windowSurface);

		SDL_UpdateWindowSurface(window);

		//LIMIT FPS
		unsigned int fps = 60;
		if ((1000 / fps) > SDL_GetTicks() - tick) {
			SDL_Delay(1000 / fps - (SDL_GetTicks() - tick));
		}
	}

	SDL_FreeSurface(npc_jamie.playerSurface);
	npc_jamie.playerSurface = nullptr;
	SDL_FreeSurface(player.playerSurface);
	player.playerSurface = nullptr;
	SDL_DestroyWindow(window);
	windowSurface = nullptr;
	return 1;
}