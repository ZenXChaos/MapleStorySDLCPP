#pragma warning (disable : 4996)
#include <stdio.h>
#include <string>

#include <tinyxml2.h>

#include <SDL.h>
#include <SDL_image.h>
#include <map>
#include <vector>
#include <random>

#include "KEENIO_SDK.h"

using namespace tinyxml2;
using namespace std;

#include "SPRITE_ANIMATION.h"
#include "player.h"
#include "MOB_ENTITY.h"
#include "NPC.h"
#include "GAME.h"
#include "map.h"

void gameRan() {
	KEENIO_CLIENT* kCLIENT = new KEENIO_CLIENT();
	kCLIENT->kHTTP.reqURL = "https://api.keen.io/3.0/projects/5717fd9380a7bd63683e73b1/events/startup";
	kCLIENT->kHTTP.addDefHeaders();

	kCLIENT->kHTTP.addParam("api_key", "");
	string data[2] = { "app_event","xLAUNCH" };
	kCLIENT->kHTTP.addDataParam("data", data);
	kCLIENT->method(KEENIO_HTTP_GET);
	kCLIENT->request(kCLIENT->kHTTP);

	printf("%s\n",kCLIENT->body.c_str());
}

int main(int argc, char* argv[]) {
	gameRan();
	//return 1;
	SDL_Init(SDL_INIT_EVERYTHING);


	SDL_Window* window = nullptr;
	SDL_Surface* windowSurface = nullptr;

	SDL_Renderer* gRenderer = NULL;

	window = SDL_CreateWindow("Hi", 50, 50, 679, 376, SDL_WINDOW_SHOWN);
	//windowSurface = SDL_GetWindowSurface(window);
	gRenderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);

	PLAYER player(gRenderer);
	//Uint32 color = SDL_MapRGB(windowSurface->format, 0xff, 0xff, 0xff);
	Uint32 tick;
	bool running = true;
	float frame = 0.0f;

	//std::vector<void*> ent;

	//NPC_JAMIE npc_jamie(windowSurface);

	//npc_jamie.winSurface = windowSurface;



	//Initialize renderer color
	SDL_SetRenderDrawColor(gRenderer, 0xFF, 0xFF, 0xFF, 0xFF);

	//Initialize PNG loading
	int imgFlags = IMG_INIT_PNG;
	if (!(IMG_Init(imgFlags) & imgFlags))
	{
		printf("SDL_image could not initialize! SDL_image Error: %s\n", IMG_GetError());
	}

	MAP home_map(gRenderer);
	GAME game(gRenderer);
	game.loadMobList();

	//SDL_FillRect(windowSurface, NULL, SDL_MapRGB(windowSurface->format, 255, 255, 255));
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
		/*

		SDL_FillRect(windowSurface, &windowSurface->clip_rect, color);

		npc_jamie.setTarget(player);
		npc_jamie.scanTarget();
		npc_jamie.collider.findCollision(player.playerRect);

		npc_jamie.playAnimation(windowSurface);

		SDL_UpdateWindowSurface(window);
		*/

		//Clear screen
		SDL_RenderClear(gRenderer);

		home_map.displayMap(windowSurface);
		player.playAnimation(windowSurface);
		player.playerMotorize(event);
		frame += 0.1f;
		game.displayAllMobs(windowSurface, &player);
		//Render texture to screen
		//SDL_RenderCopy(gRenderer, gTexture, NULL, NULL);

		//Update screen
		SDL_RenderPresent(gRenderer);

		//LIMIT FPS
		unsigned int fps = 60;
		if ((1000 / fps) > SDL_GetTicks() - tick) {
			SDL_Delay(1000 / fps - (SDL_GetTicks() - tick));
		}
	}

	/*SDL_FreeSurface(npc_jamie.playerSurface);
	npc_jamie.playerSurface = nullptr;
	SDL_FreeSurface(player.playerSurface);
	player.playerSurface = nullptr;
	SDL_DestroyWindow(window);
	windowSurface = nullptr;*/
	return 1;
}