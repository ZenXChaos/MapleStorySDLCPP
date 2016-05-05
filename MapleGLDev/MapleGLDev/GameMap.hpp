class GameMapObject {
	AnimatedVBO sprite;
public:
	int OBJECT_ID;
	LFRect pos;
};

class GameMap {
	AnimatedVBO mapSprite;
	std::map<int, GameMapObject> mapObjects;
	
public:

	void InitMap(std::string initMapFile, LFRect pos);
	void DrawMap(LFRect pos);
};