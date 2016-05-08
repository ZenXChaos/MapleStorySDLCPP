#ifndef GAME_H
#define GAME_H

#include <map>
#include <string>
#include <SDL.h>
#include "GameObject.h"
#include "Entity.hpp"
#include "SpawnManager.hpp"
#include "Box.hpp"


class Game {

	std::map<std::string, Entity> MobList;
	std::map<std::string, Entity> MOBS_LIST;
	std::map<int, std::string> MOBS_MAPPING;
	std::map<std::string, int> MOBS_MAPPINGSTRING;

	std::map<std::string, GameItemDrop> gameItemDrops;
	std::vector<GameItemDrop> mapItemDrops; // Items dropped in map

	SDL_Renderer* mainRenderer;

	Player* mainPlayer;
public:
	SpawnManager spawn_manager;

	void LoadMobList(SDL_Renderer* gRenderer);
	void LoadPlayerAnims(SDL_Renderer* gRenderer, Player* ent);
	void LoadItemDrops(SDL_Renderer* gRenderer);
	void InitSpawnManager();
	void ManageMobPool();
	void SetMainPlayer(Player* mp);
	void ManageMapObjects();
	void LoadHUDSprites(SDL_Renderer* gRenderer);
	Player* GetMainPlayer();

	b2World* gameWorld;

	Box tmpBox;
	Game() {
		b2Vec2 gravity(0.0f, 9.81f);
		this->gameWorld = new b2World(gravity);

		b2BodyDef ground;
		ground.position.Set(0, 500);

		b2Body* groundBody;
		groundBody = gameWorld->CreateBody(&ground);

		b2PolygonShape groundBox;
		groundBox.SetAsBox(50.0f, 10.0f);
		groundBody->CreateFixture(&groundBox, 0.0f);

		tmpBox.Init(gameWorld, { 1, 1 }, { 1.0, 1.0f });

	}
};
#endif

#ifdef M_RENDERER
SDL_Renderer* m_gRenderer = NULL;
#undef M_RENDERER
#else
extern SDL_Renderer* m_gRenderer;
#endif