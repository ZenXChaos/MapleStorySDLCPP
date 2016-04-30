#ifndef HUD_H
#define HUD_H
class HUDObject {
public:
	int row = 0, column = 0;

	AnimatedSprite* sprites;
};

class HUD_GridPanel {
public:
	int columns = 0, rows = 0;
	int width = 0, height = 0;

	std::map<std::string, HUDObject> elements;

	void AddObject(std::string elem_name, HUDObject obj);

	void DrawPanel(int x, int y);
};

class HUD_FlowPanel {
public:
	int width = 0, height = 0;
	int spacingY = 5, spacingX = 5;

	std::vector<HUDObject> elements;

	void AddObject(HUDObject obj);

	void DrawPanel(int x, int y);
};
static class HUD
{
public:
	HUD();
	virtual ~HUD();

} UIHUD;
#endif