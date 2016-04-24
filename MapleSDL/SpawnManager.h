
class SpawnManager {
	Uint32 lastSpawn = 0;
	Uint32 lastSpawnIndex = 0;
	size_t SpawnEvery = 3;

	SDL_Rect spawnPoints[10] = {{0,0,0,0}};

	size_t maxSpawn = 3;
public:
	std::map<std::string, Entity>* MobList;
	std::vector<Entity> spawned;

	void ManagePool(Uint32 tick);
};