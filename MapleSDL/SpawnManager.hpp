#ifndef SPAWNMANAGER_H
#define SPAWNMANAGER_H

#include <map>
#include <vector>
#include <SDL.h>

#include "Entity.hpp"


class SpawnManager {
	Uint32 lastSpawn = 0;
	Uint32 lastSpawnIndex = 0;
	size_t SpawnEvery = 10;

	SDL_Rect spawnPoints[10] = { { 0,0,0,0 } };

	size_t maxSpawn = 3;
public:
	std::map<std::string, Entity>* MobList;
	std::vector<Entity> spawned;

	void ManagePool(Uint32 tick);
	bool overridebusy = false;
	void SpawnMob();
	void SpawnMob_Safe();
	bool overridespawn;
};
#endif
