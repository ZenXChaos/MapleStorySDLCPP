#define GAMEPROP

#include "LUtil.h"
#include <IL/il.h>
#include <IL/ilu.h>
#include "LTexture.h"

#include <stdio.h>
#include <stdlib.h>
#include <string>
#include <map>
#include <vector>
#include <tinyxml2.h>

#pragma comment(lib, "tinyxml2.lib")

using namespace std;

#include "GameUtils.hpp"
#include "RelativeSpace.hpp"
#include "MessageDispatch.hpp"
#include "ItemDrop.hpp"
#include "Input.hpp"
#include "Entity.hpp"
#include "SpawnManager.hpp"
#include "Game.hpp"

Entity mobmush;

void Game::SetMainPlayer(Player* mp) {
	this->mainPlayer = mp;
}

Player* Game::GetMainPlayer() {
	return this->mainPlayer;
}

void InitGameMGR() {

	
}

void GameRun(int val) {

}

void Game::LoadMobList() {
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

		(*this->MobList)[mob_name] = new Entity();


		tinyxml2::XMLElement* mRoot = pRoot->FirstChildElement("animatedVBO");

		for (; mRoot != nullptr; mRoot = mRoot->NextSiblingElement("animatedVBO")) {
			tinyxml2::XMLElement* aRoot = mRoot->FirstChildElement("sprite");
			std::string sprite_anim_name = mRoot->Attribute("name");
			int sprite_x = mRoot->IntAttribute("x");
			int sprite_y = mRoot->IntAttribute("y");

			for (; aRoot != nullptr; aRoot = aRoot->NextSiblingElement("sprite")) {
			std::string sprite_filepath = aRoot->Attribute("file");
				float sprite_delta = mRoot->FloatAttribute("delta");
				
				(*this->MobList)[mob_name]->animations[sprite_anim_name].AddSprite(sprite_filepath + ".png", sprite_delta);
				(*this->MobList)[mob_name]->animations[sprite_anim_name].xfactor = sprite_x;
				(*this->MobList)[mob_name]->animations[sprite_anim_name].yfactor = sprite_y;

			}
		}
		
		tinyxml2::XMLElement* i_Root = pRoot->FirstChildElement("itemDrops")->FirstChildElement("item");

		for (; i_Root != nullptr; i_Root = i_Root->NextSiblingElement("item")) {
			(*MobList)[mob_name]->ItemDrops.AddItem(this->gameItemDrops[i_Root->Attribute("value")]);
		}

		int i = 0;
		for (std::map<std::string, Entity*>::iterator it = MobList->begin(); it != MobList->end(); it++) {

			MOBS_LIST[it->first.c_str()] = *it->second;
			MOBS_MAPPING[i] = it->first;
			MOBS_MAPPINGSTRING[it->first] = i;
			i++;
		}

	}
}

Entity* Game::IdentifyMob(std::string mobname) {
	std::map<std::string, Entity*> moblist = *MobList;
	std::map<std::string, int>::iterator it = MOBS_MAPPINGSTRING.find(mobname);
	if (it != MOBS_MAPPINGSTRING.end()) {
		return &MOBS_LIST[it->first];
	}

	printf("Could not identify MOB `%s`!\n", mobname.c_str());
	return nullptr;
}

void Game::LoadItemDrops() {
	tinyxml2::XMLDocument doc;
	tinyxml2::XMLElement* pRoot;
	doc.LoadFile("data\\items\\items.zenx");

	pRoot = doc.FirstChildElement("items")->FirstChildElement("item");

	for (; pRoot != nullptr; pRoot = pRoot->NextSiblingElement("item")) {
		GameItemDrop i_Drop;
		i_Drop.i_Health = pRoot->FirstChildElement("health")->IntAttribute("value");
		i_Drop.i_Mana = pRoot->FirstChildElement("mana")->IntAttribute("value");
		i_Drop.i_dropRate = pRoot->FirstChildElement("dropRate")->IntAttribute("value");
		i_Drop.i_Name = pRoot->Attribute("name");
		i_Drop.i_ID = pRoot->IntAttribute("id");

		i_Drop.sprite.AddSprite(pRoot->FirstChildElement("sprite")->Attribute("file"), 1);

		this->gameItemDrops[i_Drop.i_Name] = i_Drop;

	}


}

void Game::InitSpawnManager() {
	//Initialize spawn_manager mob list with list from Game.
	spawn_manager.MobList = new std::map<std::string, Entity*>(*this->MobList);
}

Entity* Game::IdentifyMob(int mobid) {
	std::map<std::string, Entity*> moblist = *MobList;
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
		if (this->spawn_manager.spawned[i]->alive == false) {
			LFRect tmpPos = this->spawn_manager.spawned.at(i)->GetPosition();
			tmpPos.y += 30;
			this->spawn_manager.spawned.at(i)->ItemDrops.DropItems(&this->mapItemDrops, tmpPos);
			//this->spawn_manager.spawned->erase(this->spawn_manager.spawned->begin() + i);
			break;
		}
		else {
			spawn_manager.spawned[i]->AI();
			spawn_manager.spawned[i]->Draw();
		}
	}
	//mainPlayer->IdentifyMobs();
}

void Game::ManageMapObjects() {
	//Show Drops
	for (size_t i = 0; i < this->mapItemDrops.size(); i++) {
		this->mapItemDrops[i].sprite.Animate(this->mapItemDrops[i].i_dropPos.x, this->mapItemDrops[i].i_dropPos.y, FlipDirection::Left);
	}
}

void Game::LoadPlayerAnims(Entity* e) {
	tinyxml2::XMLElement* pRoot;
	tinyxml2::XMLDocument doc;

	doc.LoadFile("data\\player_anims.xml");

	pRoot = doc.FirstChild()->FirstChildElement("animatedVBO");


	for (; pRoot != nullptr; pRoot = pRoot->NextSiblingElement("animatedVBO")) {
		std::string sprite_anim_name = pRoot->Attribute("name");
		float sprite_delta = pRoot->FloatAttribute("delta");

		tinyxml2::XMLElement* aRoot = pRoot->FirstChildElement("sprite");

		for (; aRoot != nullptr; aRoot = aRoot->NextSiblingElement("sprite")) {
			std::string sprite_filepath = aRoot->Attribute("file");
			e->animations[sprite_anim_name].AddSprite(sprite_filepath + ".png", sprite_delta);
		}
	}

	return;
}