class GAME {

	std::map<std::string, Entity>* MobList = new std::map<std::string, Entity>();
	std::map<std::string, Entity> MOBS_LIST;
	std::map<int, std::string> MOBS_MAPPING;
	std::map<std::string, int> MOBS_MAPPINGSTRING;
public:
	SpawnManager spawn_manager;

	Entity* IdentifyMob(std::string mobname);
	Entity* IdentifyMob(int mobid);
	void LoadMobList();
	void LoadPlayerAnims(std::map<std::string, AnimatedVBO>* animlist);
	void InitSpawnManager();
	void ManageMobPool();
};