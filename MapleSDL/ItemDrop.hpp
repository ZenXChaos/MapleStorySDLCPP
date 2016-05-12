#ifndef GAMEITEMDROP_H
#define GAMEITEMDROP_H
class GameItemDrop : public GAMEObject {

public:
	AnimatedSprite sprite;
	std::string i_Name;
	std::string uniq_id = "";
	int i_ID = 0; // Item ID
	int i_Health = 0; // Increase Health
	int i_Mana = 0; // Increase mana

	SDL_Rect i_dropPos = { 0,0,0,0 };

	void Core() override;

	int i_dropRate = 0; // From 0 - 100, chance of item dropping

	GameItemDrop() {
		this->uniq_id = GameUtils::UniqID() + GameUtils::UniqID();
	}
};


class GameItemDropProvider {
	std::vector<GameItemDrop> dropItems; // Possible Drops
public:
	bool hasDrops = false; // Entity has drops?

	void AddItem(GameItemDrop* i_Drop);
	void DropItems(SDL_Rect pos);
};
#endif