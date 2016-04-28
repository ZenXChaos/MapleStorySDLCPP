#include "LUtil.h"
#include <IL/il.h>
#include <IL/ilu.h>
#include "LTexture.h"

#include <stdio.h>
#include <stdlib.h>
#include <string>
#include <map>
#include <vector>

using namespace std;

#include "GameUtils.h"
#include "RelativeSpace.h"
#include "MessageDispatch.h"
#include "ItemDrop.hpp"
#include "Entity.h"
#include "SpawnManager.h"

void SpawnManager::ManagePool(GLint tick) {
	this->lastSpawn = (tick / 1000) - lastSpawnIndex;
	std::vector<Entity> mobl = this->spawned;
	if (static_cast<size_t>(this->lastSpawn) > this->SpawnEvery && this->spawned.size() < this->maxSpawn) {
		this->lastSpawnIndex = (tick / 1000);
		Entity *tmpMob = new Entity(this->MobList->at("mush"));

		this->spawned.insert(this->spawned.end(), *tmpMob);
		this->spawned.at(this->spawned.size() - 1).SetPositionY(210);
	}
	else if (this->spawned.size() >= this->maxSpawn) {
		this->lastSpawnIndex = (tick / 1000);
	}
}