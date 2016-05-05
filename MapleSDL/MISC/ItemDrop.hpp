#ifndef GAMEITEMDROP_H
#define GAMEITEMDROP_H
#include <string>
#include <vector>
#include "../AnimatedSprite.hpp"

class GameItemDrop {

public:
	AnimatedSprite sprite;
	std::string i_Name;
	int i_ID = 0; // Item ID
	int i_Health = 0; // Increase Health
	int i_Mana = 0; // Increase mana

	SDL_Rect i_dropPos = { 0,0,0,0 };

	int i_dropRate = 0; // From 0 - 100, chance of item dropping
};


class GameItemDropProvider {
	std::vector<GameItemDrop> dropItems; // Possible Drops
public:
	bool hasDrops = false; // Entity has drops?

	void AddItem(GameItemDrop i_Drop);
	void DropItems(std::vector<GameItemDrop>* gid_Item, SDL_Rect pos);
};
#endif