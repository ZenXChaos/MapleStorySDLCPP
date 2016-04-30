#ifndef ENTITY_H
#define ENTITY_H

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

	int mesoDropMin = 0;
	int mesoDropMax = 0;
	int expGain = 0;

	bool usesAccel;
	bool roaming = false;
	bool chasing = false;
	bool attacking = false;

	SDL_Rect nextTransitLocation = { 0,0,0,0 };
	Uint32 roamDelayIndex = 0;
	Uint32 tick = 0;
	Uint32 birth = 0;

	float recoveryIndex = 0.0f;
	Uint32 lastAttack = 0;
	Uint32 attackRecovery = 2;

	SDL_Rect* currFrameData = new SDL_Rect();
	AnimatedSprite* currentAnimation;

public:
	EntityState State;
	EntityType e_Type = EntityType::e_Mob;
	MessageDispatch dispatch_message;

	EntityLife Life;
	GameItemDropProvider ItemDrops;

	Uint32 roamDelay = 3;
	FlipDirection Direction = Left;
	std::map<std::string, AnimatedSprite> animations;

	bool alive = true;

	void Draw();
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

	std::string uniq_id;

	Uint32 age() {
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

	int exp = 0;
public:

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