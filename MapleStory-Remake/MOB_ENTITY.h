class MOB : public ENTITY {

public:
	MOB() : ENTITY() {
	}
};

class MOB_ENTITY : public ENTITY {

	XMLElement * pRoot;


	int FaceDirection = 0;


	void loadAnims(void) {
		tinyxml2::XMLDocument doc;

		doc.LoadFile("player_anims.xml");

		pRoot = doc.FirstChild()->FirstChildElement("anim");

		for (; pRoot != nullptr; pRoot = pRoot->NextSiblingElement("anim")) {
			std::string sprite_anim_name = pRoot->Attribute("name");
			std::string sprite_filepath = pRoot->Attribute("sprite");
			float sprite_delta = pRoot->FloatAttribute("delta");
			float sprite_max_frames = pRoot->FloatAttribute("max_frames");
			int sprite_width = pRoot->IntAttribute("sprite_width");
			int sprite_height = pRoot->IntAttribute("sprite_height");

			anims[sprite_anim_name.c_str()].display_surface = NULL;
			anims[sprite_anim_name.c_str()].display_surface = IMG_Load(sprite_filepath.c_str());
			anims[sprite_anim_name.c_str()].max_frames = sprite_max_frames;
			anims[sprite_anim_name.c_str()].delta = sprite_delta;

			if (anims[sprite_anim_name.c_str()].display_surface == NULL) {
				printf("SDL Error: %s", SDL_GetError());
			}
			else {
				int tmp_f = static_cast<int>(sprite_max_frames);
				addAnimation(&anims[sprite_anim_name.c_str()], 0, tmp_f, sprite_width, sprite_height);
			}
		}

		try {
			current_animation = &anims[doc.FirstChild()->FirstChildElement("default_state")->Attribute("name")];
		}
		catch (...) {
			printf("Error setting default animation! Assuming idle_left has been added!\n");
		}
	}

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
	PLAYER* target;

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

	bool targetIsRightOf() {
		return (target->playerRect.x > this->playerRect.x);
	}

	bool targetIsLeftOf() {
		return (target->playerRect.x < this->playerRect.x);
	}
	void scanTarget(std::string entity_name = "entity") {
		collider.setCollider(playerRect, 1, 2, winSurface);

		if (targetIsLeftOf() == true) {
			if (lastPositionOf != 1) {
				printf("Left of target (%s)!\n", entity_name.c_str());
				if (state == idle) {
					current_animation = &anims["idle_left"];
				}

				lastPositionOf = 1;
			}
		}
		else {
			if (lastPositionOf != 0) {
				printf("Right of target (%s)!\n", entity_name.c_str());
				if (state == idle) {
					current_animation = &anims["idle_right"];
				}
				lastPositionOf = 0;
			}
		}
	}

	void setTarget(PLAYER* pl) {
		target = pl;
	}
	int lastPositionOf = 0;

	MOB_ENTITY() : ENTITY() {
		//loadAnims();
		//playerRect->x = 395;
		//playerRect->y = 214;
	}
};