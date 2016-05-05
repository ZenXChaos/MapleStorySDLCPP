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
	Idle=0, Walking=1, Attacking=3, Recovery=4, Death=5
};

enum EntityType {
	e_Player=0, e_Mob=1, e_NPC=2
};

class EntityLife {

public:
	int Health = 100;
};

class Entity {
protected:
	SDL_Rect pos;
	SDL_RendererFlip FaceDirection = SDL_FLIP_NONE;

	int walkSpeed = 0;
	int maxWalkSpeed = 1;
	int walkSpeedAccel = 1;
	int minRoamTransit = 100;


	bool usesAccel;
	bool roaming = false;
	bool chasing = false;
	bool attacking = false;

	SDL_Rect nextTransitLocation = { 0,0,0,0 };
	SDL_Rect* currFrameData = new SDL_Rect();

	Uint32 birth = 0;

	float roamDelayIndex = 0;
	float tick = 0;
	float recoveryIndex = 0.0f;
	float lastAttack = 0.0f;
	float attackRecovery = 0.4f;

	AnimatedSprite* currentAnimation;

public:
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

	bool alive = true;

	void Draw(bool oc = false);
	void Walk(FlipDirection direction);
	void WalkAway(SDL_Rect topos);
	void WalkTowards(SDL_Rect topos);
	void Station();
	void Roam();
	void AI();
	void TakeHit();
	void Kill();
	void PrepKill();
	void GenUniqID();
	void DrawHealth();

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

	Entity(){
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
	void ManageState();

	Player(std::vector<Entity>* sp, Input* pi) : Entity(){
		spawned = sp;
		playerInput = pi;
		this->e_Type = EntityType::e_Player;
		GLOBAL_MMORPG_GAME::m_Player = (this);
	}
};

#endif