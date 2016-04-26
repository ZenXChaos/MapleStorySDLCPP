#include <SDL.h>
#include <SDL_image.h>
#include <stdio.h>
#include <stdlib.h>
#include <string>
#include <map>
#include <vector>
#include <Box2D\Box2D.h>

using namespace std;

#include "MessageDispatch.h"
#include "Box.h"
#include "GameUtils.h"
#include "RelativeSpace.h"
#include "AnimatedSprite.h"
#include "Entity.h"
#include "Input.h"
#include "SpawnManager.h"
#include "GameMap.h"
#include "Game.h"
#include "HelperFunctions.h"

#undef main

int main(int argc, char* argv) {
	SDL_Init(SDL_INIT_EVERYTHING);


	SDL_Window* window = nullptr;

	SDL_Renderer* gRenderer = NULL;

	window = SDL_CreateWindow("Hi", 50, 50, 1000, 500, SDL_WINDOW_SHOWN);
	gRenderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);


	Uint32 tick;
	bool pauseReset = true;
	bool running = true;
	float frame = 0.0f;


	//Initialize renderer color
	SDL_SetRenderDrawColor(gRenderer, 0xFF, 0xFF, 0xFF, 0xFF);

	//Initialize PNG loading
	int imgFlags = IMG_INIT_PNG;
	if (!(IMG_Init(imgFlags) & imgFlags))
	{
		printf("SDL_image could not initialize! SDL_image Error: %s\n", IMG_GetError());
	}

	Game game;
	Entity entity;
	
	game.LoadMobList(gRenderer);
	game.LoadPlayerAnims(gRenderer, &entity);
	game.InitSpawnManager();


	//IDENFITY A MOB
	//Entity mush = *game.IdentifyMob("mush");

	Input PlayerInput;

	GameMap map;
	SDL_Rect mapPos;
	mapPos.w = 1387;
	mapPos.h = 907;
	mapPos.x = 0;
	mapPos.y = -407;
	map.InitMap("content\\maps\\hennesys\\map01.png", mapPos, gRenderer);
	entity.SetPositionY(190);
	while (running) {

		tick = SDL_GetTicks();
		SDL_Event event;

		while (SDL_PollEvent(&event)) {
			switch (event.type) {
			case SDL_QUIT:
				break;
			case SDL_KEYDOWN:
				PlayerInput.KeyDown(event.key.keysym.scancode);
				break;
			case SDL_KEYUP:
				PlayerInput.KeyUp(event.key.keysym.scancode);
				break;
			}
		}

		//Clear screen
		SDL_RenderClear(gRenderer);

		if (PlayerInput.IsKeyPressed(SDL_SCANCODE_LEFT)) {
			entity.Walk(Left);
		}
		else if (PlayerInput.IsKeyPressed(SDL_SCANCODE_RIGHT)) {
			entity.Walk(Right);
		}
		else {
			entity.Station();
		}

		map.DrawMap(mapPos);
		entity.Draw();
		game.ManageMobPool();

		//Update screen
		SDL_RenderPresent(gRenderer);

		//SpawnManager
		game.spawn_manager.ManagePool(tick);

		//LIMIT FPS
		frame += 0.1f;
		unsigned int fps = 60;
		if ((1000 / fps) > SDL_GetTicks() - tick) {
			SDL_Delay(1000 / fps - (SDL_GetTicks() - tick));
		}
	}

	SDL_DestroyWindow(window);
	return 1;
}

