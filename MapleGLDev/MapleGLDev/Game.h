class Game {

	std::map<std::string, Entity> MOBS_LIST;
	std::map<int, std::string> MOBS_MAPPING;
	std::map<std::string, int> MOBS_MAPPINGSTRING;

	std::map<std::string, GameItemDrop> gameItemDrops;
	std::vector<GameItemDrop> mapItemDrops; // Items dropped in map

	Player* mainPlayer;
public:
	std::map<std::string, Entity*>* MobList ;
	SpawnManager spawn_manager;

	Entity* IdentifyMob(std::string mobname);
	Entity* IdentifyMob(int mobid);
	void LoadMobList();
	void LoadPlayerAnims(Entity* e);
	void LoadItemDrops();
	void InitSpawnManager();
	void ManageMobPool();
	void SetMainPlayer(Player* mp);
	void ManageMapObjects();
	Player* GetMainPlayer();
};

#ifndef GAMEPROP
extern int groundY;
#else
int groundY = 400;
#undef GAMEPROP
#endif