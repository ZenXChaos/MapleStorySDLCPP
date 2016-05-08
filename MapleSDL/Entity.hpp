#ifndef ENTITY_H
#define ENTITY_H
#include <vector>
#include <map>
#include <SDL.h>
#include "AnimatedSprite.hpp"
#include "MessageDispatch.hpp"
#include "RelativeSpace.hpp"
#include "GameUtils.hpp"
#include "Input.hpp"
#include "MISC/ItemDrop.hpp"

enum EntityState {
	Idle = 0, Walking = 1, Attacking = 3, Recovery = 4, Death = 5, Chasing = 6
};

enum EntityType {
	e_Player=0, e_Mob=1, e_NPC=2
};

class EntityLife {

public:
	int Health = 100;
};

class Entity : public GAMEObject {
protected:
	SDL_RendererFlip FaceDirection = SDL_FLIP_NONE;

	int walkSpeed = 0;
	int walkSpeedAccel = 1;
	int minRoamTransit = 100;


	bool usesAccel;
	bool roaming = false;
	bool chasing = false;
	bool attacking = false;
	bool alert = false;

	SDL_Rect nextTransitLocation = { 0,0,0,0 };
	SDL_Rect* currFrameData = new SDL_Rect();

	Uint32 birth = 0;

	float roamDelayIndex = 0;
	float tick = 0;
	float recoveryIndex = 0.0f;
	float lastAttack = 0.0f;
	float attackRecovery = 0.05f;

	AnimatedSprite* currentAnimation;

public:
	SDL_Rect pos;

	EntityState State;
	EntityType e_Type = EntityType::e_Mob;
	MessageDispatch dispatch_message;

	EntityLife Life;
	GameItemDropProvider ItemDrops;

	Uint32 lastHit = 0;
	Uint32 roamDelay = 3;
	FlipDirection Direction = Left;
	std::map<std::string, AnimatedSprite> animations;

	int mesoDropMin = 0;
	int mesoDropMax = 0;
	int expGain = 0;

	int maxWalkSpeed = 1;

	bool alive = true;

	void Draw(bool oc = false);
	void Walk(FlipDirection direction);
	void WalkAway(SDL_Rect topos);
	void WalkTowards(SDL_Rect topos);
	void Station();
	void Roam();
	void TakeHit();
	void Kill();
	void PrepKill();
	void GenUniqID();
	void DrawHealth();	
	void Core() override;
	void ApplyDamage(int dmg) { this->Life.Health -= dmg; if (this->Life.Health <= 0) { this->PrepKill(); } }


	std::string uniq_id;

	float age() {
		return ((this->tick/1000) - this->birth);
	}

	int GetHeight();
	int GetWidth();
	void SetPositionX(int y);
	void SetPositionY(int y);
	SDL_Rect GetPosition();
	int GetPositionX();
	int GetPositionY();

	Entity() : GAMEObject(){
		this->FaceDirection = SDL_FLIP_HORIZONTAL;
		this->birth = SDL_GetTicks()/1000;
		this->uniq_id = GameUtils::UniqID() + GameUtils::UniqID();
		//this->dispatch_message.RegisterMessage("Hit", &IsHit, this);
	}
};

class Player : public Entity {
	int attackRange=100;

	Entity* closestMob = nullptr;
	Input* playerInput;

public:

	int expPts = 0;

	std::vector<Entity>* spawned;
	std::vector<Entity*> inRange;

	void IdentifyMobs();
	void AttackMob();

	void Core() override;

	Player(std::vector<Entity>* sp, Input* pi) : Entity(){
		spawned = sp;
		playerInput = pi;
		this->e_Type = EntityType::e_Player;
		GLOBAL_MMORPG_GAME::m_Player = (this);
	}
};

#endif