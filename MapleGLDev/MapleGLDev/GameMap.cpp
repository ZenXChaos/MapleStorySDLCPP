#include "LUtil.h"
#include <IL/il.h>
#include <IL/ilu.h>
#include "LTexture.h"

#include <stdio.h>
#include <stdlib.h>
#include <string>
#include <map>
#include <vector>

using namespace std;

#include "GameMap.h"

void GameMap::InitMap(std::string initMapFile, LFRect pos) {
	this->mapSprite.AddSprite(initMapFile, 0.1f);
	//this->mapSprite.BuildAnimation(0, 1, pos.w, pos.h, 0.0f);
}

void GameMap::DrawMap(LFRect pos) {
	this->mapSprite.Animate(pos.x, pos.y, 0);
}