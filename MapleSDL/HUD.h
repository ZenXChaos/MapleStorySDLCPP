#pragma once
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

class HUD
{
public:
	HUD();
	virtual ~HUD();
};
#endif