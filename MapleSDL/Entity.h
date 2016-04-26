#ifndef ENTITY_H
#define ENTITY_H

enum EntityState {
	Idle=0, Walking=1, Attacking=3, Recovery=4, Death=5
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

	float recoveryIndex = 0;

	bool usesAccel;
	bool roaming = false;
	bool chasing = false;
	bool attacking = false;

	SDL_Rect nextTransitLocation = { 0,0,0,0 };
	Uint32 roamDelayIndex = 0;
	Uint32 tick;
	Uint32 birth;

	SDL_Rect* currFrameData = new SDL_Rect();
	AnimatedSprite* currentAnimation;

public:
	EntityState State;
	MessageDispatch dispatch_message;

	EntityLife Life;

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

	std::string uniq_id;

	Uint32 age() {
		return (this->tick - this->birth)/1000;
	}

	int GetHeight();
	int GetWidth();
	void SetPositionX(int y);
	void SetPositionY(int y);
	int GetPositionX();
	int GetPositionY();

	void test(void) {
		printf("%i", this->chasing);

		return;
	}

	Entity(){
		this->FaceDirection = SDL_FLIP_HORIZONTAL;
		this->birth = SDL_GetTicks();
		this->uniq_id = GameUtils::UniqID() + GameUtils::UniqID();
		//this->dispatch_message.RegisterMessage("Hit", &IsHit, this);
	}
};

class Player : public Entity {
	int attackRange=100;

	Entity* closestMob = nullptr;
	Input* playerInput;
public:

	std::vector<Entity>* spawned;
	std::vector<Entity*> inRange;

	void IdentifyMobs();
	void ManageState();

	Player(std::vector<Entity>* sp, Input* pi) : Entity(){
		spawned = sp;
		playerInput = pi;
	}
};

#endif