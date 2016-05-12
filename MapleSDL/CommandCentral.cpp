#include <SDL.h>
#include <SDL_image.h>
#include <stdio.h>
#include <iostream>
#include <stdlib.h>
#include <string>
#include <map>
#include <vector>
#include "GameDebug.hpp"
#include "Global.h"

using namespace std;

#include "Input.hpp"
#include "MessageDispatch.hpp"
#include "GameUtils.hpp"
#include "RelativeSpace.hpp"
#include "AnimatedSprite.hpp"
#include "GameObject.h"
#include "ItemDrop.hpp"
#include "HUD.hpp"
#include "SpawnManager.hpp"
#include "Entity.hpp"
#include "Camera.hpp"
#include "Map.h"
#include "Game.hpp"
#include "CommandCentral.h"

extern SpawnManager* defSpawnManager;

int CommandCentral::CommandMain(void* data)
{
	std::string cmmnd;
	while (1) {
		// Get console input
		cin >> cmmnd;

		//If command entered in console
		if (cmmnd == "/kill") {
			//Kill the game
			mainRunning = false;
		}
		else if (cmmnd == "/forcespawn") {
			//Force a mob to spawn
			defSpawnManager->SpawnMob_Safe();
		}
	}
	return 1;
}

CommandCentral::CommandCentral()
{
}


CommandCentral::~CommandCentral()
{
}
