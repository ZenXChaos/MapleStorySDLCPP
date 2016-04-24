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
	Uint32 roamDelay = 3;
	FlipDirection Direction = Left;
	std::map<std::string, AnimatedSprite>* animations= new std::map<std::string, AnimatedSprite>();

	void Draw();
	void Walk(FlipDirection direction);
	void Entity::WalkAway(SDL_Rect topos);
	void Entity::WalkTowards(SDL_Rect topos);
	void Station();
	void Roam();
	void AI();

	Uint32 age() {
		return (this->tick - this->birth)/1000;
	}

	int GetHeight();
	int GetWidth();
	void SetPositionX(int y);
	void SetPositionY(int y);

	Entity(){
		this->FaceDirection = SDL_FLIP_HORIZONTAL;
		this->birth = SDL_GetTicks();
	}
};