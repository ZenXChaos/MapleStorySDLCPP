#include <SDL.h>
#include <SDL_image.h>
#include <stdio.h>
#include <stdlib.h>
#include <string>
#include <map>
#include <vector>
#include <tinyxml2.h>
#include <Box2D/Box2D.h>
#include <SDL.h>
#include "GameDebug.hpp"
#include "Global.h"

#pragma comment(lib, "tinyxml2.lib")
//TODO: Handle path separator 

using namespace std;

#include "Input.hpp"
#include "MessageDispatch.hpp"
#include "GameUtils.hpp"
#include "RelativeSpace.hpp"
#include "AnimatedSprite.hpp"
#include "GameObject.h"
#include "ItemDrop.hpp"
#include "Entity.hpp"
#include "SpawnManager.hpp"
#include "Box.hpp"
#include "HUD.hpp"
#include "Camera.hpp"
#include "Game.hpp"


void Game::SetMainPlayer(Player* mp) {
	this->mainPlayer = mp;
}

Player* Game::GetMainPlayer() {
	return this->mainPlayer;
}

void Game::LoadMobList(SDL_Renderer* gRenderer){
	//Load mobs : `data/mobs/mobs.zenx`
	this->mainRenderer = gRenderer;
	tinyxml2::XMLDocument doc;
	tinyxml2::XMLElement* pRoot;
	doc.LoadFile("data/mobs/mobs.zenx");

	//Loop through all `mob` nodes
	for (pRoot = doc.FirstChildElement("mobs")->FirstChildElement("mob"); pRoot != nullptr; pRoot = pRoot->NextSiblingElement("mob")) {
		std::string mob_name = pRoot->Attribute("name");
		bool mob_hasDrops = pRoot->BoolAttribute("hasDrops");
		int mob_id = pRoot->IntAttribute("id");
		int mob_init_health = pRoot->IntAttribute("health");
		int mob_init_armour = pRoot->IntAttribute("armour");
		int mob_init_mana = pRoot->IntAttribute("mana");
		int mob_exp_gain = pRoot->IntAttribute("exp");
		Entity mob;
		(MobList)[mob_name] = mob;

		(MobList)[mob_name].ItemDrops.hasDrops = mob_hasDrops;

		(MobList)[mob_name].expGain = mob_exp_gain;
		tinyxml2::XMLElement* aRoot = pRoot->FirstChildElement("animations")->FirstChildElement("anim");

		//Loop through the childNodes: `anims`
		for (; aRoot != nullptr; aRoot = aRoot->NextSiblingElement("anim")) {
			std::string sprite_anim_name = aRoot->Attribute("name");
			std::string sprite_filepath = aRoot->Attribute("sprite");
			float sprite_delta = aRoot->FloatAttribute("delta");
			int sprite_max_frames = aRoot->IntAttribute("max_frames");
			int sprite_width = aRoot->IntAttribute("sprite_width");
			int sprite_height = aRoot->IntAttribute("sprite_height");
			int sprite_y_factor = aRoot->IntAttribute("yfactordown") - aRoot->IntAttribute("yfactorup") ;
			int sprite_x_factor = aRoot->IntAttribute("xfactordown") - aRoot->IntAttribute("xfactorup");

			//Create an animation state
			AnimatedSprite as;
			(MobList)[mob_name].animations.insert(std::pair<std::string, AnimatedSprite>(sprite_anim_name, as));
			(MobList)[mob_name].animations.at(sprite_anim_name.c_str()).LoadTexture(sprite_filepath.c_str(), gRenderer);
			(MobList)[mob_name].animations.at(sprite_anim_name.c_str()).BuildAnimation(0, sprite_max_frames, sprite_width, sprite_height, sprite_delta);
			(MobList)[mob_name].animations.at(sprite_anim_name.c_str()).yfactor = sprite_y_factor;
			(MobList)[mob_name].animations.at(sprite_anim_name.c_str()).xfactor = sprite_x_factor;
			if ((MobList)[mob_name].animations.at(sprite_anim_name.c_str()).texture == nullptr) {
				printf("SDL Error: %s", SDL_GetError());
			}
			
		}

		tinyxml2::XMLElement* i_Root = pRoot->FirstChildElement("itemDrops")->FirstChildElement("item");
		
		for (; i_Root != nullptr; i_Root = i_Root->NextSiblingElement("item")) {
			GameItemDrop* idrop = gameItemDrops.FindByName(i_Root->Attribute("value"));
			if (idrop == nullptr) {
				break;
			}
			else {
				(MobList)[mob_name].ItemDrops.AddItem(idrop);
			}
		}

		int i = 0;
		for (std::map<std::string, Entity>::iterator it = MobList.begin(); it != MobList.end(); it++) {
			
			MOBS_LIST[it->first.c_str()] = it->second;
			MOBS_MAPPING[i] = it->first;
			MOBS_MAPPINGSTRING[it->first] = i;
			i++;
		}

	}
}

void Game::LoadItemDrops(SDL_Renderer* gRenderer) {
	// Load ItemDrops : `data/items/items.zenx`
	this->mainRenderer = gRenderer;
	tinyxml2::XMLDocument doc;
	tinyxml2::XMLElement* pRoot;
	doc.LoadFile("data/items/items.zenx");


	//Select first `item` node
	pRoot = doc.FirstChildElement("items")->FirstChildElement("item");

	//Loop through all `item` nodes
	for (; pRoot != nullptr; pRoot = pRoot->NextSiblingElement("item")) {
		
		//Set GameItemDrop (i_Drop) properties
		GameItemDrop * i_Drop = new GameItemDrop();
		i_Drop->i_Health = pRoot->FirstChildElement("health")->IntAttribute("value");
		i_Drop->i_Mana = pRoot->FirstChildElement("mana")->IntAttribute("value");
		i_Drop->i_dropRate = pRoot->FirstChildElement("dropRate")->IntAttribute("value");
		i_Drop->i_Name = pRoot->Attribute("name");
		i_Drop->i_ID = pRoot->IntAttribute("id");

		i_Drop->sprite.LoadTexture(pRoot->FirstChildElement("sprite")->Attribute("file"), gRenderer);
		i_Drop->sprite.BuildAnimation(0, 1, pRoot->FirstChildElement("sprite")->IntAttribute("width"), pRoot->FirstChildElement("sprite")->IntAttribute("height"), 0.1f);
		
		//Instantiate the new GAMEObject
		gameItemDrops.Instantiate(i_Drop);

	}


}

void Game::InitSpawnManager() {
	//Initialize spawn_manager mob list with list from Game.
	spawn_manager.MobList = new std::map<std::string, Entity>(this->MobList);
}

void Game::ManageMobPool() {

	// Loop through all spawned mobs
	for (size_t i = 0; i < spawn_manager.spawned.size(); i++) {
		if (i >= spawn_manager.spawned.size()) {
			break; //just in case?
		}

		//If the entity is dead
		if (this->spawn_manager.spawned[i].alive == false) {
			SDL_Rect tmpPos = this->spawn_manager.spawned.at(i).GetPosition();
			tmpPos.y += 30;

			// Remove it from the spawn list
			//Drop an item
			this->spawn_manager.spawned.at(i).ItemDrops.DropItems(tmpPos);
			this->spawn_manager.spawned.erase(this->spawn_manager.spawned.begin()+i);
			break;
		}

		else{
			if (&spawn_manager.spawned[i].Life != nullptr) {
				
				// If within camera view, draw to the screen. Otherwise just set animation data
				if (spawn_manager.spawned[i].GetPositionX() < MainCamera.pos.w+MainCamera.pos.x) {
					spawn_manager.spawned[i].Draw();
				}else{
					spawn_manager.spawned[i].Draw(true);
				}

				//Run the mobs CORE HANDLE
				spawn_manager.spawned[i].Core();
			}
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
}

void Game::ManageMapObjects() {
	//Show Drops
	std::for_each(begin(gameItems.objects), end(gameItems.objects), [](auto& drop) -> void {
		GameItemDrop* dp = drop;

		if (dp == nullptr) {
			return;
		}
		dp->sprite.Animate(dp->i_dropPos, 0, NULL, SDL_RendererFlip::SDL_FLIP_NONE);
	});
}

void Game::LoadHUDSprites(SDL_Renderer* gRenderer)
{
	this->mainRenderer = gRenderer;
	tinyxml2::XMLDocument doc;
	tinyxml2::XMLElement* pRoot;
	doc.LoadFile("data/HUD.zenx");


	for (pRoot = doc.FirstChildElement("HUD")->FirstChildElement("HUDSprite"); pRoot != nullptr; pRoot = pRoot->NextSiblingElement("HUDSprite")) {
		std::string sprite_name = pRoot->Attribute("name");
		HUDElements[sprite_name].LoadTexture(pRoot->Attribute("sprite"), gRenderer);
		HUDElements[pRoot->Attribute("name")].BuildAnimation(0, 1, pRoot->IntAttribute("sprite_width"), pRoot->IntAttribute("sprite_height"), 0);
	}
}

void Game::LoadPlayerAnims(SDL_Renderer* gRenderer, Player* ent) {
	//Load player animations : `data/player_anims.xml`
	tinyxml2::XMLElement* pRoot;
	tinyxml2::XMLDocument doc;

	doc.LoadFile("data/player_anims.xml");

	//Loop through all `anim` nodes
	for (pRoot = doc.FirstChild()->FirstChildElement("anim"); pRoot != nullptr; pRoot = pRoot->NextSiblingElement("anim")) {
		std::string sprite_anim_name = pRoot->Attribute("name");
		std::string sprite_filepath = pRoot->Attribute("sprite");
		float sprite_delta = pRoot->FloatAttribute("delta");
		int sprite_max_frames = pRoot->IntAttribute("max_frames");
		int sprite_width = pRoot->IntAttribute("sprite_width");
		int sprite_height = pRoot->IntAttribute("sprite_height");

		//Create an animated sprite from properties
		//Load texture
		AnimatedSprite as;
		ent->animations[sprite_anim_name] = as;
		ent->animations[sprite_anim_name].LoadTexture(sprite_filepath.c_str(), gRenderer);
		
		if (ent->animations[sprite_anim_name].texture == NULL) {
			//File probably not found
			//SDL_image PNG may not be initialized
			//SDL_Renderer might not be set
			printf("SDL Error: %s\n", SDL_GetError());
			return;
		}
		else {
			//If texture loaded, build the animation
			int tmp_f = static_cast<int>(sprite_max_frames);
			ent->animations[sprite_anim_name].BuildAnimation(0, sprite_max_frames, sprite_width, sprite_height, sprite_delta);
			printf("Animation built and loaded: %s\n", sprite_anim_name.c_str());
		}
	}

	return ;
}