#ifndef DYNAMIC2DCHAR_H
#define DYNAMIC2DCHAR_H
class Dynamic2DBodyPart {

public:
	int relX, relY;
	int footHoldX = 0, footHoldY = 0;
	std::map<std::string, AnimatedSprite> bp_Animations;
};

class Dynamic2DCharacter : public Player
{
public:
	std::map<std::string, Dynamic2DBodyPart> d2_BodyParts;

	void DrawParts(SDL_Rect worldPos);
	Dynamic2DCharacter(Player* mainPlayer);
	virtual ~Dynamic2DCharacter();
};
#endif