

class ENTITY_LIFE {

public:
	int Health = 0;
	int Armour = 0;
	int Mana = 0;
	float eop = 0.0f;

	Uint32 LifeTime = 0;
	Uint32 Birth = 0;
};


enum PLAYER_STATE {
	idle = 1, walking = 2, attack = 3
};

class ENTITY {

protected:
	SDL_Rect animclips[10];
	float max_frames = 0;
	float current_frame = 0.0f;


	void addAnimation(SDL_Rect* clip, int row, int cnt, int w, int h)
	{
		for (int i = 0; i < cnt - 1; i += 1) {
			clip[i].x = 0 + i * w;
			clip[i].y = row * h;
			clip[i].w = w;
			clip[i].h = h;
		}
	}

public:

	PLAYER_STATE state = idle;
	KeyInput* KeyboardInput = new KeyInput();
	SDL_Surface* playerSurface = nullptr;
	SDL_Rect playerRect;
	float deltaTime = 0.0f;
	SDL_Surface* winSurface;

	void sendMessage(std::string message, void* sender) {
		printf("Collision message received: %s\n", message.c_str());
	}

	void playAnimation(SDL_Surface* windowSurface) {
		if (current_frame >= max_frames - 1) {
			current_frame = 0;
		}
		SDL_BlitSurface(playerSurface, &animclips[static_cast<int>(current_frame)], windowSurface, &playerRect);


		current_frame += deltaTime;

	}

	ENTITY() {

	}
};

class MOB : public ENTITY {

public:
	MOB() : ENTITY() {
	}
};

class MOB_ENTITY : public ENTITY {

	XMLElement * pRoot;


	int FaceDirection = 0;


	int state_trans = 0;

	float nextRoam = 0.0f;
	bool roamInTransit = false;
	SDL_Rect nextTransitLocation;
public:
	SDL_Rect playerRect;
	std::map<std::string, SPRITE_ANIMATION> anims;
	SPRITE_ANIMATION* current_animation;
	std::string MOB_NAME;
	int MOB_ID, ENTITY_ID=0;
	ENTITY_LIFE Life;

	void extended_state_handler() {
		if (state == attack) {

		}
	}

	void setAttackMode() {
		current_animation->active = false;
		this->state = attack;
		current_animation = (FaceDirection == 0) ? (&anims["attack01_left"]) : (&anims["attack01_right"]);
		current_animation->current_frame = 0;
		current_animation->active = true;

	}

	void roamAround() {
		if (roamInTransit == false) {
			if (nextRoam < 3.0f) {

				nextRoam += 0.02f;
			}
			else {
				roamInTransit = true;
				std::random_device rd;     // only used once to initialise (seed) engine
				std::mt19937 rng(rd());    // random-number engine used (Mersenne-Twister in this case)
				std::uniform_int_distribution<int> uni(200, 500); // guaranteed unbiased

				int random_integer = uni(rng);
				nextTransitLocation.x = random_integer;
				printf("Mob roam: Random x location = %i\n", random_integer);
			}
		}
		else {
			if (playerRect.x < nextTransitLocation.x) {

				current_animation = &anims["walk_right"];
				FaceDirection = 1;

				playerRect.x++;
			}
			else if (playerRect.x > nextTransitLocation.x) {

				current_animation = &anims["walk_left"];
				FaceDirection = 0;
				playerRect.x--;
			}
			else {
				roamInTransit = false;
				nextRoam = 0.0f;

				if (FaceDirection == 0) {
					current_animation = &anims["idle_left"];
				}
				else {
					current_animation = &anims["idle_right"];
				}
			}
		}
	}

	void resetAttackMode() {
		this->state = attack;
	}

	void addAnimation(SPRITE_ANIMATION* animr, int row, int cnt, int w, int h)
	{
		for (int i = 0; i < cnt; i += 1) {
			animr->animRects[i].x = 0 + i * w;
			animr->animRects[i].y = row * h;
			animr->animRects[i].w = w;
			animr->animRects[i].h = h;
		}
	}

	void playAnimation(SDL_Surface* windowSurface) {
		/*if (current_frame >= current_animation->max_frames - 1) {
		current_frame = 0;
		}
		SDL_BlitSurface(current_animation->display_surface, &current_animation->animRects[static_cast<int>(current_frame)], windowSurface, playerRect);


		current_frame += current_animation->delta;
		current_animation->current_frame = current_frame;*/

	}

	int lastPositionOf = 0;

	MOB_ENTITY() : ENTITY() {
		//loadAnims();
		//playerRect->x = 395;
		//playerRect->y = 214;
	}
};