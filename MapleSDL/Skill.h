#pragma once
class Skill : public GAMEObject
{
	SDL_Rect bindedEffectPos = { 0 };
public:
	Entity* target;
	AnimatedSprite sprite;

	Uint32 healthEffector = -5;

	void BindEntity(Player* p) {
		target = &(*p);
		this->BindEffectPos(this->target->pos);
	}

	void BindEntity(Entity* e) {
		target = e;
		this->BindEffectPos(this->target->pos);
	}

	void BindEffectPos(SDL_Rect pos) {
		this->bindedEffectPos = pos;
	}

	void Draw(SDL_Rect pos);
	void Core() override;
	void OnCreate() override;

	Skill();
	virtual ~Skill();
};

