#pragma once
class Skill
{
public:
	AnimatedSprite sprite;

	Uint32 healthEffector = -5;

	void Draw(SDL_Rect pos);

	Skill();
	virtual ~Skill();
};

