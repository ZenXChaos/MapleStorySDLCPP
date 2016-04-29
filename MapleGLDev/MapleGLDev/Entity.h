
enum EntityState {
	Idle = 0, Walking = 1, Attacking = 3, Recovery = 4, Death = 5
};

class EntityLife {

public:
	int Health = 100;
};

class Entity {
protected:

	int walkSpeed = 0;
	int maxWalkSpeed = 1;
	int walkSpeedAccel = 1;
	int minRoamTransit = 100;

	float recoveryIndex = 0;

	bool usesAccel;
	bool roaming = false;
	bool chasing = false;
	bool attacking = false;

	LFRect nextTransitLocation = { 0,0,0,0 };
	GLint roamDelayIndex = 0;
	GLint tick = 0;
	GLint birth = 0;

	LFRect* currFrameData = new LFRect();
	AnimatedVBO* currentAnimation;



public:
	LFRect* pos = new LFRect();
	EntityState State;
	MessageDispatch dispatch_message;

	EntityLife Life;
	GameItemDropProvider ItemDrops;

	GLint roamDelay = 3;
	FlipDirection Direction = FlipDirection::Left;
	std::map<std::string, AnimatedVBO> animations ;

	bool alive = true;

	void Draw();
	void Walk(FlipDirection direction);
	void WalkAway(LFRect topos);
	void WalkTowards(LFRect topos);
	void Station();
	void Roam();
	void AI();
	void TakeHit();
	void Kill();
	void PrepKill();
	void Tick();

	std::string uniq_id = "";

	GLint age() {
		return (this->tick - this->birth)/60;
	}

	GLfloat GetHeight();
	GLfloat GetWidth();
	void SetPositionX(GLfloat y);
	void SetPositionY(GLfloat y);
	LFRect GetPosition();
	GLfloat GetPositionX();
	GLfloat GetPositionY();

	Entity() {
		//this->FaceDirection = SDL_FLIP_HORIZONTAL;
		//this->birth = SDL_GetTicks();
		this->uniq_id = GameUtils::UniqID() + GameUtils::UniqID();
	}
};

class Player : public Entity {
	int attackRange = 100;

	Entity* closestMob = nullptr;
	//Input* playerInput;
public:

	std::vector<Entity*>* spawned;
	std::vector<Entity*> inRange;

	void IdentifyMobs();
	void ManageState();

	Player(std::vector<Entity*>* sp) : Entity() {
		spawned = sp;
		//playerInput = pi;
	}
};