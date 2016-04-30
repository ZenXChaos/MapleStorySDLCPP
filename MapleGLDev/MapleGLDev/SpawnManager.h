
class SpawnManager {
	GLint lastSpawn = 0;
	GLint lastSpawnIndex = 0;
	size_t SpawnEvery = 10;

	LFRect spawnPoints[10] = {{0,0,0,0}};

	size_t maxSpawn = 3;
public:
	std::map<std::string, Entity*>* MobList;
	std::vector<Entity*> spawned;

	void ManagePool(GLint tick);
};