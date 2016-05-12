#include <SDL.h>
#include <SDL_image.h>
#include <stdio.h>
#include <stdlib.h>
#include <string>
#include <map>
#include <vector>
#include <algorithm>
#include "GameDebug.hpp"
#include "Global.h"

using namespace std;

#include "Input.hpp"
#include "MessageDispatch.hpp"
#include "GameUtils.hpp"
#include "RelativeSpace.hpp"
#include "AnimatedSprite.hpp"
#include "GameObject.h"
#include "ItemDrop.hpp"
#include "HUD.hpp"
#include "Entity.hpp"
#include "Camera.hpp"
#include "Skill.h"

#include "ItemDrop.hpp"

void GameItemDropProvider::AddItem(GameItemDrop* i_Drop) {
	this->dropItems.insert(this->dropItems.end(), *i_Drop);
	this->hasDrops = true;
}

void GameItemDropProvider::DropItems(SDL_Rect pos) {
	//Loop through all possible drop items
	for (size_t i = 0; i < this->dropItems.size(); i++) {
		//Drop rate / Chance of drop (%)
		int rate = this->dropItems.at(i).i_dropRate;

		GameItemDrop *gid = new GameItemDrop();
		memcpy(&gid->sprite, &gameItemDrops.FindByName(this->dropItems.at(i).i_Name)->sprite, sizeof(gameItemDrops.FindByName(this->dropItems.at(i).i_Name)->sprite));
		gid->sprite = this->dropItems.at(i).sprite;
		gid->i_dropPos = pos;

		//Generate random number, if less than chance rate, drop item
		if (GameUtils::RandomIntegerRange(0, 100) <= rate) {
			gameItemDrops.Instantiate(gid);
		}
	}
}

void GameItemDrop::Core()
{

}
