#include <stdio.h>
#include <stdlib.h>
#include <string>
#include <map>
#include <vector>
#include "LUtil.h"
#include <IL/il.h>
#include <IL/ilu.h>
#include "LTexture.h"

#include "RelativeSpace.h"
#include "GameUtils.h"
#include "ItemDrop.hpp"

void GameItemDropProvider::AddItem(GameItemDrop i_Drop) {
	this->dropItems.insert(this->dropItems.end(), i_Drop);
	this->hasDrops = true;
}

void GameItemDropProvider::DropItems(std::vector<GameItemDrop>* gid_Item, LFRect pos) {
	for (size_t i = 0; i < this->dropItems.size(); i++) {
		int rate = this->dropItems.at(i).i_dropRate;

		GameItemDrop gid = this->dropItems[i];
		gid.i_dropPos = pos;
		if (GameUtils::RandomIntegerRange(0, 100) <= rate) {
			gid_Item->insert(gid_Item->end(), gid);
		}
	}
}