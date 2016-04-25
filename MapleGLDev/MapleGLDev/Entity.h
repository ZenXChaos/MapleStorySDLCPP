enum EntityState {
	Idle = 0, Walking = 1, Attacking = 3, Recovery = 4
};

class Entity {
	LFRect pos;
	//SDL_RendererFlip FaceDirection = SDL_FLIP_NONE;
	EntityState State;

	int walkSpeed = 0;
	int maxWalkSpeed = 1;
	int walkSpeedAccel = 1;
	int minRoamTransit = 100;

	bool usesAccel;
	bool roaming = false;
	bool chasing = false;

	LFRect nextTransitLocation = { 0,0,0,0 };
	GLint roamDelayIndex = 0;
	GLint tick;
	GLint birth;

	LFRect* currFrameData = new LFRect();

public:
	GLint roamDelay = 3;
	FlipDirection Direction = FlipDirection::Left;
	std::map<std::string, AnimatedVBO>* animations = new std::map<std::string, AnimatedVBO>();

	void Draw();
	void Walk(FlipDirection direction);
	void Entity::WalkAway(LFRect topos);
	void Entity::WalkTowards(LFRect topos);
	void Station();
	void Roam();
	void AI();

	GLint age() {
		return (this->tick - this->birth) / 1000;
	}

	GLfloat GetHeight();
	GLfloat GetWidth();
	void SetPositionX(GLfloat y);
	void SetPositionY(GLfloat y);

	Entity() {
		//this->FaceDirection = SDL_FLIP_HORIZONTAL;
		//this->birth = SDL_GetTicks();
	}
};