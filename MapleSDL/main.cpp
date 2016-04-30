#include <SDL.h>
#include <SDL_image.h>
#include <stdio.h>
#include <stdlib.h>
#include <string>
#include <map>
#include <vector>
#include <Box2D\Box2D.h>
#include "GameDebug.h"
#include "Global.h"

using namespace std;


#include "Input.h"
#include "MessageDispatch.h"
#include "Box.h"
#include "GameUtils.h"
#include "RelativeSpace.h"
#include "AnimatedSprite.h"
#include "MISC\ItemDrop.hpp"
#include "Entity.hpp"
#include "SpawnManager.h"
#include "GameMap.h"
#include "Game.h"
#include "HelperFunctions.h"
#include "HUD.h"

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
	Input PlayerInput;
	Player entity(&game.spawn_manager.spawned, &PlayerInput);
	
	game.SetMainPlayer(&entity);
	game.LoadItemDrops(gRenderer);
	game.LoadMobList(gRenderer);
	game.LoadPlayerAnims(gRenderer, &entity);
	game.InitSpawnManager();


	//IDENFITY A MOB
	//Entity mush = *game.IdentifyMob("mush");


	GameMap map;
	SDL_Rect mapPos;
	mapPos.w = 1387;
	mapPos.h = 907;
	mapPos.x = 0;
	mapPos.y = -407;
	map.InitMap("content\\maps\\hennesys\\map01.png", mapPos, gRenderer);
	entity.SetPositionY(190);

	HUD_FlowPanel hudgrid;
	hudgrid.height = 100;
	hudgrid.width = 50;

	HUD_FlowPanel hudgrid2;
	hudgrid2.height = 100;
	hudgrid2.width = 10;
	//hudgrid.columns = 5;
	//hudgrid.rows = 1;

	AnimatedSprite as1;
	as1.LoadTexture("content\\misc\\itemNo\\ItemNo.1.png", gRenderer);
	as1.BuildAnimation(0, 1, 8, 11, 0.1f);

	AnimatedSprite as2;
	as2.LoadTexture("content\\misc\\itemNo\\ItemNo.2.png", gRenderer);
	as2.BuildAnimation(0, 1, 8, 11, 0.1f);
	//AnimatedSprite as3;
	//as1.LoadTexture("content\\misc\\itemNo\\ItemNo.3.png", gRenderer);

	HUDObject hob1;
	hob1.sprites = &as1;
	hob1.column = 0;
	hob1.row = 0;
	HUDObject hob2;
	hob2.sprites = &as2;
	hob2.column = 1;
	hob2.row = 0;
	//HUDObject hob3;
	//hob3.sprites = &as3;

	hudgrid.AddObject("1", hob1);
	hudgrid.AddObject("2", hob2);

	hudgrid2.AddObject("1", hob1);
	hudgrid2.AddObject("2", hob2);
	//hudgrid.AddObject("3", hob3);



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
		entity.ManageState();
		entity.Draw();
		game.ManageMobPool();
		game.ManageMapObjects();

		//HUD
		hudgrid.DrawPanel(10, 10);
		hudgrid2.DrawPanel(10, 22);
		
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

