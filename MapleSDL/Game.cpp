#include <SDL.h>
#include <SDL_image.h>
#include <stdio.h>
#include <stdlib.h>
#include <string>
#include <map>
#include <vector>
#include <tinyxml2.h>
#include <Box2D/Box2D.h>
#include <SDL2/SDL.h>
#include "GameDebug.h"
#include "Global.h"

#pragma comment(lib, "tinyxml2.lib")
//TODO: Handle path separator 

using namespace std;

#include "Input.h"
#include "MessageDispatch.h"
#include "GameUtils.h"
#include "RelativeSpace.h"
#include "AnimatedSprite.h"
#include "MISC/ItemDrop.hpp"
#include "Entity.hpp"
#include "SpawnManager.h"
#include "Box.h"
#include "Game.h"


void Game::SetMainPlayer(Player* mp) {
	this->mainPlayer = mp;
}

Player* Game::GetMainPlayer() {
	return this->mainPlayer;
}

void Game::LoadMobList(SDL_Renderer* gRenderer){
	this->mainRenderer = gRenderer;
	tinyxml2::XMLDocument doc;
	tinyxml2::XMLElement* pRoot;
	doc.LoadFile("data//mobs//mobs.zenx");

	pRoot = doc.FirstChildElement("mobs")->FirstChildElement("mob");

	for (; pRoot != nullptr; pRoot = pRoot->NextSiblingElement("mob")) {
		std::string mob_name = pRoot->Attribute("name");
		bool mob_hasDrops = pRoot->BoolAttribute("hasDrops");
		int mob_id = pRoot->IntAttribute("id");
		int mob_init_health = pRoot->IntAttribute("Health");
		int mob_init_armour = pRoot->IntAttribute("Armour");
		int mob_init_mana = pRoot->IntAttribute("Mana");
		
		Entity mob;
		//(*MobList)[mob_name] = mob;

		//(*MobList)[mob_name].ItemDrops.hasDrops = mob_hasDrops;
    MobList[mob_name] = mob;

		MobList[mob_name].ItemDrops.hasDrops = mob_hasDrops;


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
      //TODO: fix me 
      /*  
			this->MobList->at(mob_name).animations.insert(std::pair<std::string, AnimatedSprite>(sprite_anim_name, as));
			this->MobList->at(mob_name).animations.at(sprite_anim_name.c_str()).LoadTexture(sprite_filepath.c_str(), gRenderer);
			this->MobList->at(mob_name).animations.at(sprite_anim_name.c_str()).BuildAnimation(0, sprite_max_frames, sprite_width, sprite_height, sprite_delta);
			this->MobList->at(mob_name).animations.at(sprite_anim_name.c_str()).yfactor = sprite_y_factor;
			this->MobList->at(mob_name).animations.at(sprite_anim_name.c_str()).xfactor = sprite_x_factor;
    
			if (MobList->at(mob_name).animations.at(sprite_anim_name.c_str()).texture == nullptr) {
				printf("SDL Error: %s", SDL_GetError());
			}
      */
			this->MobList.at(mob_name).animations.insert(std::pair<std::string, AnimatedSprite>(sprite_anim_name, as));
			this->MobList.at(mob_name).animations.at(sprite_anim_name.c_str()).LoadTexture(sprite_filepath.c_str(), gRenderer);
			this->MobList.at(mob_name).animations.at(sprite_anim_name.c_str()).BuildAnimation(0, sprite_max_frames, sprite_width, sprite_height, sprite_delta);
			this->MobList.at(mob_name).animations.at(sprite_anim_name.c_str()).yfactor = sprite_y_factor;
			this->MobList.at(mob_name).animations.at(sprite_anim_name.c_str()).xfactor = sprite_x_factor;
    
      //TODO: fix me
      /*
			if (MobList->at(mob_name).animations.at(sprite_anim_name.c_str()).texture == nullptr) {
				printf("SDL Error: %s", SDL_GetError());
			}
      */
      if (MobList.at(mob_name).animations.at(sprite_anim_name.c_str()).texture == nullptr) {
				printf("SDL Error: %s", SDL_GetError());
			}
		}

		tinyxml2::XMLElement* i_Root = pRoot->FirstChildElement("itemDrops")->FirstChildElement("item");
		
		for (; i_Root != nullptr; i_Root = i_Root->NextSiblingElement("item")) {
      //TODO: fix me
			//(*MobList)[mob_name].ItemDrops.AddItem(this->gameItemDrops[i_Root->Attribute("value")]);
			MobList[mob_name].ItemDrops.AddItem(this->gameItemDrops[i_Root->Attribute("value")]);
		}

		int i = 0;
    //TODO: fix me
		//for (std::map<std::string, Entity>::iterator it = MobList->begin(); it != MobList->end(); it++) {
		for (std::map<std::string, Entity>::iterator it = MobList.begin(); it != MobList.end(); it++) {
			
			MOBS_LIST[it->first.c_str()] = it->second;
			MOBS_MAPPING[i] = it->first;
			MOBS_MAPPINGSTRING[it->first] = i;
			i++;
		}

	}
}

void Game::LoadItemDrops(SDL_Renderer* gRenderer) {
	this->mainRenderer = gRenderer;
	tinyxml2::XMLDocument doc;
	tinyxml2::XMLElement* pRoot;
  //TODO: add guard around 
	doc.LoadFile("data//items//items.zenx");

	pRoot = doc.FirstChildElement("items")->FirstChildElement("item");

	for (; pRoot != nullptr; pRoot = pRoot->NextSiblingElement("item")) {
		GameItemDrop i_Drop;
		i_Drop.i_Health = pRoot->FirstChildElement("health")->IntAttribute("value");
		i_Drop.i_Mana = pRoot->FirstChildElement("mana")->IntAttribute("value");
		i_Drop.i_dropRate = pRoot->FirstChildElement("dropRate")->IntAttribute("value");
		i_Drop.i_Name = pRoot->Attribute("name");
		i_Drop.i_ID = pRoot->IntAttribute("id");

		i_Drop.sprite.LoadTexture(pRoot->FirstChildElement("sprite")->Attribute("file"), gRenderer);
		i_Drop.sprite.BuildAnimation(0, 1, pRoot->FirstChildElement("sprite")->IntAttribute("width"), pRoot->FirstChildElement("sprite")->IntAttribute("height"), 0.1f);

		this->gameItemDrops[i_Drop.i_Name] = i_Drop;

	}


}

Entity* Game::IdentifyMob(std::string mobname) {
	//std::map<std::string, Entity> moblist = *MobList;
	std::map<std::string, int>::iterator it = MOBS_MAPPINGSTRING.find(mobname);
	if (it != MOBS_MAPPINGSTRING.end()) {
		return &MOBS_LIST[it->first];
	}

	printf("Could not identify MOB `%s`!\n", mobname.c_str());
	return nullptr;
}

void Game::InitSpawnManager() {
	//Initialize spawn_manager mob list with list from Game.
  //TODO: fix me
	//spawn_manager.MobList = new std::map<std::string, Entity>(*this->MobList);
	spawn_manager.MobList = new std::map<std::string, Entity>(this->MobList);
}

Entity* Game::IdentifyMob(int mobid) {
	//std::map<std::string, Entity> moblist = *MobList;
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
		if (this->spawn_manager.spawned[i].alive == false) {
			SDL_Rect tmpPos = this->spawn_manager.spawned.at(i).GetPosition();
			tmpPos.y += 30;
			this->spawn_manager.spawned.at(i).ItemDrops.DropItems(&this->mapItemDrops, tmpPos);
			this->spawn_manager.spawned.erase(this->spawn_manager.spawned.begin()+i);
			break;
		}
		else {
			spawn_manager.spawned[i].AI();
			spawn_manager.spawned[i].Draw();
		}
	}

	gameWorld->Step(1.0f, 56, 80);

	SDL_Rect testBox2D;
	testBox2D.x = static_cast<int>(this->tmpBox.getBody()->GetPosition().x);
	testBox2D.y = static_cast<int>(this->tmpBox.getBody()->GetPosition().y);
	testBox2D.w = 20;
	testBox2D.h = 20;

	SDL_SetRenderDrawColor(this->mainRenderer, 0xFF, 0x00, 0x00, 0xFF);
	SDL_RenderDrawRect(this->mainRenderer, &testBox2D);
	SDL_RenderFillRect(this->mainRenderer, &testBox2D);
	SDL_RenderDrawRect(this->mainRenderer, &testBox2D);
	SDL_SetRenderDrawColor(this->mainRenderer, 0xFF, 0xFF, 0xFF, 0xFF);

	mainPlayer->IdentifyMobs();
}

void Game::ManageMapObjects() {
	//Show Drops
	for (size_t i = 0; i < this->mapItemDrops.size(); i++) {
		this->mapItemDrops[i].sprite.Animate(this->mapItemDrops[i].i_dropPos, 0, NULL, SDL_FLIP_NONE, nullptr);
	}
}

void Game::LoadPlayerAnims(SDL_Renderer* gRenderer, Player* ent) {
	tinyxml2::XMLElement* pRoot;
	tinyxml2::XMLDocument doc;

	doc.LoadFile("data//player_anims.xml");

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
