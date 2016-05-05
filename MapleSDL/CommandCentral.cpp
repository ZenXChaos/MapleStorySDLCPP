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
#include "MISC\ItemDrop.hpp"
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
		cin >> cmmnd;

		if (cmmnd == "/kill") {
			//Lock
			SDL_SemWait(mainLock);
			mainRunning = false;
			SDL_SemPost(mainLock);
		}
		else if (cmmnd == "/forcespawn") {

			SDL_SemWait(mainSpawnMGRLock);
			defSpawnManager->SpawnMob_Safe();
			SDL_SemPost(mainSpawnMGRLock);
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
