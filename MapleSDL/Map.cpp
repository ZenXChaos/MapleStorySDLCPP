#include <SDL.h>
#include <SDL_image.h>
#include <stdio.h>
#include <stdlib.h>
#include <string>
#include <map>
#include <vector>
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
#include "Map.h"
#include "Game.hpp"

#include <tinyxml2.h>



void Map::LoadMapTiles(std::string filename)
{
	tinyxml2::XMLDocument doc;
	tinyxml2::XMLElement* pRoot;
	doc.LoadFile(filename.c_str());


	for (pRoot = doc.FirstChild()->FirstChildElement("imgdir"); pRoot != nullptr; pRoot = pRoot->NextSiblingElement("imgdir")) {
		MapTile tile;
		std::string u = pRoot->FirstChildElement("u")->Attribute("value");
		int x = pRoot->FirstChildElement("x")->IntAttribute("value");
		int y = pRoot->FirstChildElement("y")->IntAttribute("value");
		int no = pRoot->FirstChildElement("no")->IntAttribute("value");

		std::string texturepath = "content/maps/map01/Tiles/woodMarble/" + u + ".";

		char *maptno = (char*)malloc(sizeof(char) * 80);
		_itoa_s(no, maptno, 80, 10);

		texturepath += maptno;
		texturepath += ".png";

		tile.tile.LoadTexture(texturepath.c_str(), m_gRenderer);
		SDL_QueryTexture(tile.tile.texture, NULL, NULL, &tile.w, &tile.h);
		tile.tile.BuildAnimation(0, 1, tile.w, tile.h, 0);

		tile.u = u;
		tile.x = x;
		tile.y = y;

		this->tiles.insert(this->tiles.end(), tile);



	}

}

void Map::DrawMap()
{
	std::vector<MapTile>::iterator it;

	for (it = this->tiles.begin(); it != this->tiles.end(); it++) {
		SDL_Rect tilePos;
		tilePos.x = it->x;
		tilePos.y = it->y;
		tilePos.w = it->w;
		tilePos.h = it->h;

		it->tile.Animate(tilePos, 0, NULL, SDL_RendererFlip::SDL_FLIP_NONE, nullptr);
	}
}

Map::Map()
{
}


Map::~Map()
{
}
