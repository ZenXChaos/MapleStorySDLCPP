#include <SDL.h>
#include <SDL_image.h>
#include <stdio.h>
#include <stdlib.h>
#include <string>
#include <map>
#include <vector>
#include <tinyxml2.h>
#include <Box2D/Box2D.h>
#include "GameDebug.h"
#include "Global.h"

#pragma comment(lib, "tinyxml2.lib")

using namespace std;

#include "SpawnManager.h"
#include "Input.h"
#include "MessageDispatch.h"
#include "Box.h"
#include "GameUtils.h"
#include "RelativeSpace.h"
#include "AnimatedSprite.h"
#include "MISC/ItemDrop.hpp"
#include "Game.h"

void SpawnManager::ManagePool(Uint32 tick) {
	this->lastSpawn = (tick/1000) - lastSpawnIndex;
	std::vector<Entity> mobl = this->spawned;
	if (static_cast<size_t>(this->lastSpawn) > this->SpawnEvery && this->spawned.size() < this->maxSpawn) {
		this->lastSpawnIndex = (tick/1000);
		Entity *tmpMob = new Entity(this->MobList->at("mush"));

		this->spawned.insert(this->spawned.end(), *tmpMob);
		this->spawned.at(this->spawned.size() - 1).SetPositionY(210);
		this->spawned.at(this->spawned.size() - 1).GenUniqID();
		GLOBAL_MMORPG_GAME::EntityDispatchedHandles.RegisterEntity(this->spawned.at(this->spawned.size() - 1).uniq_id)->RegisterEntityHandle(this->spawned.at(this->spawned.size() - 1).uniq_id, &this->spawned.at(this->spawned.size() - 1).dispatch_message);
	}
	else if (this->spawned.size() >= this->maxSpawn) {
		this->lastSpawnIndex = (tick / 1000);
	}
}