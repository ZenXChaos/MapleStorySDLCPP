
class SPAWN_MANAGER {

	Uint32 lastSpawn = 0;
	Uint32 indexTime = 0;
	Uint32 indexTime2 = 0;
	Uint32 spawnEvery=5;
	int maxmobspawn = 5;
public:
	std::map<std::string, MOB_ENTITY>* MOBS;
	std::map<int, std::string>* MOBS_LIST;

	std::vector<MOB_ENTITY> spawned;

	size_t tmpSize=0;
	bool busy = false;

	void manage(AUTOHITBOX* hbox) {

		indexTime = SDL_GetTicks() / 1000;

		if (lastSpawn >= spawnEvery && static_cast<Uint32>(maxmobspawn) > spawned.size()) {
			//indexTime = 0;
			busy = true;
			indexTime2 = indexTime;
			spawnMob(MOBS->at("mush"));
			MOB_ENTITY* mobe;
			mobe = &spawned.at(spawned.size()-1);
			
			hbox->bindBoxToRect(static_cast<void*>(mobe), spawned.size()-1);
			
		}

		lastSpawn = indexTime - indexTime2;
	}

	void spawnMob(MOB_ENTITY & mob) {
		/*MOB_ENTITY mob ;
		
		
		memcpy(static_cast<void*>(&mob), static_cast<void*>(&MOBS->at(mob_name)), sizeof(MOB_ENTITY));
		mob.current_animation = &mob.anims["idle_left"];
		*/
		mob.ENTITY_ID = spawned.size();
		spawned.insert(spawned.end(), mob);
		

		SDL_Delay(1000);
		busy = false;
		tmpSize++;
	}

};

SPAWN_MANAGER spawnmgr;

int spawn_manage(void* data);