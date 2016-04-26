#ifndef ENTITY_H
#define ENTITY_H

enum EntityState {
	Idle=0, Walking=1, Attacking=3, Recovery=4
};

class Entity {
	SDL_Rect pos;
	SDL_RendererFlip FaceDirection = SDL_FLIP_NONE;
	EntityState State;

	int walkSpeed = 0;
	int maxWalkSpeed = 1;
	int walkSpeedAccel = 1;
	int minRoamTransit = 100;

	bool usesAccel;
	bool roaming = false;
	bool chasing = false;

	SDL_Rect nextTransitLocation = { 0,0,0,0 };
	Uint32 roamDelayIndex = 0;
	Uint32 tick;
	Uint32 birth;

	SDL_Rect* currFrameData = new SDL_Rect();

public:
	MessageDispatch dispatch_message;

	Uint32 roamDelay = 3;
	FlipDirection Direction = Left;
	std::map<std::string, AnimatedSprite> animations;

	void Draw();
	void Walk(FlipDirection direction);
	void Entity::WalkAway(SDL_Rect topos);
	void Entity::WalkTowards(SDL_Rect topos);
	void Station();
	void Roam();
	void AI();

	std::string uniq_id;

	Uint32 age() {
		return (this->tick - this->birth)/1000;
	}

	int GetHeight();
	int GetWidth();
	void SetPositionX(int y);
	void SetPositionY(int y);

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

#endif