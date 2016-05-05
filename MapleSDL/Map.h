class MapTile {
public:
	std::string u = "";
	int x = 0;
	int y = 0;
	int w = 0;
	int h = 0;
	int no = 0;

	AnimatedSprite tile;
};

class Map
{
public:
	std::vector<MapTile> tiles;

	void LoadMapTiles(std::string filename);
	void DrawMap();
	Map();
	virtual ~Map();
};

