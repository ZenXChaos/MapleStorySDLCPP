#ifndef GAMEMAP_H
#define GAMEMAP_H
class GameMapObject {
	AnimatedSprite sprite;
public:
	int OBJECT_ID;
	SDL_Rect pos;
};

class GameMap {
	AnimatedSprite mapSprite;
	std::map<int, GameMapObject> mapObjects;
	
public:

	void InitMap(std::string initMapFile, SDL_Rect pos, SDL_Renderer* gRenderer);
	void DrawMap(SDL_Rect pos);
};
#endif