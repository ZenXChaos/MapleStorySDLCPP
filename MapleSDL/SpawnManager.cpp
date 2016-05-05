#include <SDL.h>
#include <SDL_image.h>
#include <stdio.h>
#include <stdlib.h>
#include <string>
#include <map>
#include <vector>
#include <tinyxml2.h>
#include <Box2D/Box2D.h>
#include "GameDebug.hpp"
#include "Global.h"

#pragma comment(lib, "tinyxml2.lib")

using namespace std;

#include "SpawnManager.hpp"
#include "Input.hpp"
#include "MessageDispatch.hpp"
#include "Box.hpp"
#include "GameUtils.hpp"
#include "RelativeSpace.hpp"
#include "AnimatedSprite.hpp"
#include "MISC/ItemDrop.hpp"
#include "Game.hpp"

void SpawnManager::ManagePool(Uint32 tick) {
	if (this->overridebusy == false)
		SDL_SemWait(mainSpawnMGRLock);
		this->lastSpawn = (tick / 1000) - lastSpawnIndex;
		std::vector<Entity> mobl = this->spawned;
		if (static_cast<size_t>(this->lastSpawn) > this->SpawnEvery && this->spawned.size() < this->maxSpawn || this->overridespawn) {
			this->lastSpawnIndex = (tick / 1000);
			this->SpawnMob();
			this->overridespawn = false;
		}
		else if (this->spawned.size() >= this->maxSpawn) {
			this->lastSpawnIndex = (tick / 1000);
		}
		SDL_SemPost(mainSpawnMGRLock);
	}

void SpawnManager::SpawnMob()
{
	try {
		Entity *tmpMob = new Entity(this->MobList->at("mush"));

		this->spawned.insert(this->spawned.end(), *tmpMob);
		this->spawned.at(this->spawned.size() - 1).SetPositionY(405);
		this->spawned.at(this->spawned.size() - 1).GenUniqID();
		GLOBAL_MMORPG_GAME::EntityDispatchedHandles.RegisterEntity(this->spawned.at(this->spawned.size() - 1).uniq_id)->RegisterEntityHandle(this->spawned.at(this->spawned.size() - 1).uniq_id, &this->spawned.at(this->spawned.size() - 1).dispatch_message);
		this->lastSpawnIndex = (SDL_GetTicks() / 1000);
	}
	catch (...) {
		printf("Failed to spawn mob!\n");
	}
}

void SpawnManager::SpawnMob_Safe()
{
	SDL_SemWait(mainSpawnMGRLock);
	this->overridespawn = true;
	SDL_SemPost(mainSpawnMGRLock);
}