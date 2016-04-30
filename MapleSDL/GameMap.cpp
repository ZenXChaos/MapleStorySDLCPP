#include <SDL.h>
#include <SDL_image.h>
#include <stdio.h>
#include <stdlib.h>
#include <string>
#include <map>
#include <vector>
#include "GameDebug.h"
#include "Global.h"

using namespace std;

#include "AnimatedSprite.h"
#include "GameMap.h"
#include "HUD.h"

void GameMap::InitMap(std::string initMapFile, SDL_Rect pos, SDL_Renderer* gRenderer) {
	this->mapSprite.LoadTexture(initMapFile, gRenderer);
	this->mapSprite.BuildAnimation(0, 1, pos.w, pos.h, 0.0f);
}

void GameMap::DrawMap(SDL_Rect pos) {
	this->mapSprite.Animate(pos, 0, NULL, SDL_FLIP_NONE, nullptr);
}