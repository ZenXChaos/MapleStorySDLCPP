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

#include "GameUtils.hpp"
#include "RelativeSpace.hpp"
#include "MessageDispatch.hpp"
#include "ItemDrop.hpp"
#include "Input.hpp"
#include "Entity.hpp"
#include "SpawnManager.hpp"

void SpawnManager::ManagePool(GLint tick) {
	this->lastSpawn = (tick/60 )- lastSpawnIndex;
	if (static_cast<size_t>(this->lastSpawn) > this->SpawnEvery && this->spawned.size() < this->maxSpawn) {
		this->lastSpawnIndex = (tick / 60);
		Entity tmpMob ;
		//tmpMob.animations = (*this->MobList)["mush"]->animations;
		tmpMob.alive = true;

		Entity* tmpM = new Entity();
		(*tmpM).animations = (*this->MobList)["mush"]->animations;
		memcpy(tmpM, (*this->MobList)["mush"], sizeof((*this->MobList)["mush"]));
		this->spawned.insert(this->spawned.end(), tmpM);
		this->spawned.at(this->spawned.size() - 1)->pos = new LFRect();
		//this->spawned.at(this->spawned.size() - 1).SetPositionY(210);
	}
	else if (this->spawned.size() >= this->maxSpawn) {
		this->lastSpawnIndex = (tick / 60);
	}
}