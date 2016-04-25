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
#include "Entity.h"
#include "SpawnManager.h"

void SpawnManager::ManagePool(GLint tick) {
	this->lastSpawn = (tick/1000) - lastSpawnIndex;
	if (static_cast<size_t>(this->lastSpawn) > this->SpawnEvery && this->spawned.size() < this->maxSpawn) {
		this->lastSpawnIndex = (tick/1000);
		this->spawned.insert(this->spawned.end(), this->MobList->at("mush"));
		this->spawned.at(this->spawned.size() - 1).SetPositionY(210);
	}
}