#include <SDL.h>
#include <SDL_image.h>
#include <stdio.h>
#include <stdlib.h>
#include <string>
#include <map>
#include <vector>
#include <tinyxml2.h>
#include <Box2D/Box2D.h>

#pragma comment(lib, "tinyxml2.lib")

using namespace std;

#include "GameUtils.h"
#include "RelativeSpace.h"
#include "AnimatedSprite.h"
#include "Entity.h"
#include "SpawnManager.h"
#include "Box.h"
#include "Game.h"

void Game::LoadMobList(SDL_Renderer* gRenderer){
	this->mainRenderer = gRenderer;
	tinyxml2::XMLDocument doc;
	tinyxml2::XMLElement* pRoot;
	doc.LoadFile("data\\mobs\\mobs.zenx");

	pRoot = doc.FirstChildElement("mobs")->FirstChildElement("mob");

	for (; pRoot != nullptr; pRoot = pRoot->NextSiblingElement("mob")) {
		std::string mob_name = pRoot->Attribute("name");
		int mob_id = pRoot->IntAttribute("id");
		int mob_init_health = pRoot->IntAttribute("Health");
		int mob_init_armour = pRoot->IntAttribute("Armour");
		int mob_init_mana = pRoot->IntAttribute("Mana");
		
		Entity mob;
		(*MobList)[mob_name] = mob;

		tinyxml2::XMLElement* aRoot = pRoot->FirstChildElement("animations")->FirstChildElement("anim");

		for (; aRoot != nullptr; aRoot = aRoot->NextSiblingElement("anim")) {
			std::string sprite_anim_name = aRoot->Attribute("name");
			std::string sprite_filepath = aRoot->Attribute("sprite");
			float sprite_delta = aRoot->FloatAttribute("delta");
			int sprite_max_frames = aRoot->IntAttribute("max_frames");
			int sprite_width = aRoot->IntAttribute("sprite_width");
			int sprite_height = aRoot->IntAttribute("sprite_height");
			int sprite_y_factor = aRoot->IntAttribute("yfactordown") - aRoot->IntAttribute("yfactorup") ;
			int sprite_x_factor = aRoot->IntAttribute("xfactordown") - aRoot->IntAttribute("xfactorup");

			AnimatedSprite as;
			this->MobList->at(mob_name).animations.insert(std::pair<std::string, AnimatedSprite>(sprite_anim_name, as));
			this->MobList->at(mob_name).animations.at(sprite_anim_name.c_str()).LoadTexture(sprite_filepath.c_str(), gRenderer);
			this->MobList->at(mob_name).animations.at(sprite_anim_name.c_str()).BuildAnimation(0, sprite_max_frames, sprite_width, sprite_height, sprite_delta);
			this->MobList->at(mob_name).animations.at(sprite_anim_name.c_str()).yfactor = sprite_y_factor;
			this->MobList->at(mob_name).animations.at(sprite_anim_name.c_str()).xfactor = sprite_x_factor;
			if (MobList->at(mob_name).animations.at(sprite_anim_name.c_str()).texture == nullptr) {
				printf("SDL Error: %s", SDL_GetError());
			}
			
		}

		int i = 0;
		for (std::map<std::string, Entity>::iterator it = MobList->begin(); it != MobList->end(); it++) {
			
			MOBS_LIST[it->first.c_str()] = it->second;
			MOBS_MAPPING[i] = it->first;
			MOBS_MAPPINGSTRING[it->first] = i;
			i++;
		}

	}
}

Entity* Game::IdentifyMob(std::string mobname) {
	std::map<std::string, Entity> moblist = *MobList;
	std::map<std::string, int>::iterator it = MOBS_MAPPINGSTRING.find(mobname);
	if (it != MOBS_MAPPINGSTRING.end()) {
		return &MOBS_LIST[it->first];
	}

	printf("Could not identify MOB `%s`!\n", mobname.c_str());
	return nullptr;
}

void Game::InitSpawnManager() {
	//Initialize spawn_manager mob list with list from Game.
	spawn_manager.MobList = new std::map<std::string, Entity>(*this->MobList);
}

Entity* Game::IdentifyMob(int mobid) {
	std::map<std::string, Entity> moblist = *MobList;
	std::map<int, std::string>::iterator it = MOBS_MAPPING.find(mobid);
	if (it != MOBS_MAPPING.end()) {
		return &MOBS_LIST[it->second];
	}

	printf("Could not identify MOB with ID `%i`!\n", mobid);

	Entity *nullentity = NULL;
	return nullptr;
}

void Game::ManageMobPool() {
	for (size_t i = 0; i < spawn_manager.spawned.size(); i++) {

		spawn_manager.spawned[i].Draw();
		spawn_manager.spawned[i].AI();
	}

	gameWorld->Step(1.0f, 56, 80);

	SDL_Rect testBox2D;
	testBox2D.x = this->tmpBox.getBody()->GetPosition().x;
	testBox2D.y = this->tmpBox.getBody()->GetPosition().y;
	testBox2D.w = 20;
	testBox2D.h = 20;

	SDL_SetRenderDrawColor(this->mainRenderer, 0xFF, 0x00, 0x00, 0xFF);
	SDL_RenderDrawRect(this->mainRenderer, &testBox2D);
	SDL_RenderFillRect(this->mainRenderer, &testBox2D);
	SDL_RenderDrawRect(this->mainRenderer, &testBox2D);
	SDL_SetRenderDrawColor(this->mainRenderer, 0xFF, 0xFF, 0xFF, 0xFF);

	printf("Box Pos y = %f\n", this->tmpBox.getBody()->GetPosition().y);
	printf("Box Pos x = %f\n", this->tmpBox.getBody()->GetPosition().x);
}

void Game::LoadPlayerAnims(SDL_Renderer* gRenderer, Entity* ent) {
	tinyxml2::XMLElement* pRoot;
	tinyxml2::XMLDocument doc;

	doc.LoadFile("data\\player_anims.xml");

	pRoot = doc.FirstChild()->FirstChildElement("anim");


	for (; pRoot != nullptr; pRoot = pRoot->NextSiblingElement("anim")) {
		std::string sprite_anim_name = pRoot->Attribute("name");
		std::string sprite_filepath = pRoot->Attribute("sprite");
		float sprite_delta = pRoot->FloatAttribute("delta");
		int sprite_max_frames = pRoot->IntAttribute("max_frames");
		int sprite_width = pRoot->IntAttribute("sprite_width");
		int sprite_height = pRoot->IntAttribute("sprite_height");

		AnimatedSprite as;
		ent->animations[sprite_anim_name] = as;
		ent->animations[sprite_anim_name].LoadTexture(sprite_filepath.c_str(), gRenderer);
		
		if (ent->animations[sprite_anim_name].texture == NULL) {
			printf("SDL Error: %s\n", SDL_GetError());
		}
		else {
			int tmp_f = static_cast<int>(sprite_max_frames);
			ent->animations[sprite_anim_name].BuildAnimation(0, sprite_max_frames, sprite_width, sprite_height, sprite_delta);
			printf("Animation built and loaded: %s\n", sprite_anim_name.c_str());
		}
	}

	return ;
}