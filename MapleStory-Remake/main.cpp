#include <stdio.h>

#include <SDL.h>
#include <map>
#include <vector>

#include "player.h"

int main(int argc, char* argv[]) {
	SDL_Init(SDL_INIT_EVERYTHING);

	PLAYER player;

	SDL_Window* window = nullptr;
	SDL_Surface* windowSurface = nullptr;

	window = SDL_CreateWindow("Hi", 100, 100, 300, 300, SDL_WINDOW_RESIZABLE);
	windowSurface = SDL_GetWindowSurface(window);

	Uint32 color = SDL_MapRGB(windowSurface->format, 0xff, 0xff, 0xff);
	Uint32 tick;
	bool running = true;
	float frame = 0.0f;

	std::vector<void*> ent;

	NPC_JAMIE npc_jamie(windowSurface);

	npc_jamie.winSurface = windowSurface;

	
	MOB_ENTITY shade(windowSurface, "mobs\\shadewraith\\standing\\left.bmp", 6, 696/6, 112, 30, 100, ShadeWraith);
	shade.deltaTime = 0.2f;
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
		player.playAnimation(windowSurface);
		npc_jamie.scanTarget();
		npc_jamie.setTarget(player);
		npc_jamie.collider.findCollision(player.playerRect);

		npc_jamie.playAnimation(windowSurface);
		shade.playAnimation(windowSurface);

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