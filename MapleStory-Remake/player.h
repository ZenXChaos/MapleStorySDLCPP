#pragma once

#include "hitbox.h"
#include "input.h"

class ENTITY_LIFE {

public:
	int Health = 0;
	int Armour = 0;
	int Mana = 0;
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
		for (int i = 0; i < cnt-1; i += 1) {
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
	SDL_Rect* playerRect = new SDL_Rect();
	HITBOX collider;
	float deltaTime = 0.0f;
	SDL_Surface* winSurface;

	void playAnimation(SDL_Surface* windowSurface) {
		if (current_frame >= max_frames - 1) {
			current_frame = 0;
		}
		SDL_BlitSurface(playerSurface, &animclips[static_cast<int>(current_frame)], windowSurface, playerRect);
		

		current_frame += deltaTime;
		
	}


	ENTITY() {
		
	}
};

class ENTITY_LOGIC {
	ENTITY entity;
public:
	ENTITY_LOGIC() {

	}
};

class SPRITE_ANIMATION {

public:
	float max_frames;
	float current_frame = 0.0f;
	float delta;

	bool active = true;

	SDL_Surface* display_surface;
	SDL_Rect animRects[100];

	SPRITE_ANIMATION() {
	}
};

class PLAYER : public ENTITY {
	std::map<std::string, SPRITE_ANIMATION> anims;
	SPRITE_ANIMATION* current_animation;

	XMLElement * pRoot;

	int FaceDirection = 0;

	void addAnimation(SPRITE_ANIMATION* animr, int row, int cnt, int w, int h)
	{
		for (int i = 0; i < cnt; i += 1) {
			animr->animRects[i].x = 0 + i * w;
			animr->animRects[i].y = row * h;
			animr->animRects[i].w = w;
			animr->animRects[i].h = h;
		}
	}

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
public:

	void playerMotorize(const SDL_Event & event) {
		if (this->KeyboardInput->isKeyHeld(event, SDL_SCANCODE_LEFT)) {
			playerRect->x -= 1;
			state = walking;
			if (state_trans != 1) {
				current_animation = &anims["walk_left"];
				state_trans = 1;
				current_frame = 0;
			}
			FaceDirection = 0;
		}
		else if (this->KeyboardInput->isKeyHeld(event, SDL_SCANCODE_RIGHT)) {
			playerRect->x += 1;
			state = walking;
			if (state_trans != 2) {
				current_animation = &anims["walk_right"];
				state_trans = 2;
				current_frame = 0;
			}
			FaceDirection = 1;
		}
		else if (this->KeyboardInput->isKeyHeld(event, SDL_SCANCODE_DOWN)) {
			//playerRect->y += 1;
			state = walking;
		}
		else if (this->KeyboardInput->isKeyHeld(event, SDL_SCANCODE_UP)) {
			//playerRect->y -= 1;
			state = walking;
		}
		else {
			if (state_trans != 0) {
				if (FaceDirection == 0) {
					current_animation = &anims["idle_left"];
				}
				else {
					current_animation = &anims["idle_right"];
				}
				state = idle;
			}
			state_trans = 0;
		}

		if (state == walking) {
#ifdef DEBUG_OPT_PLAYER_TRACKPOS
			printf("Player Position Updated: {%i, %i}\n", playerRect->x, playerRect->y);
#endif
		}
	}

	void playAnimation(SDL_Surface* windowSurface) {
		if (current_frame >= current_animation->max_frames-1) {
			current_frame = 0;
		}
		SDL_BlitSurface(current_animation->display_surface, &current_animation->animRects[static_cast<int>(current_frame)], windowSurface, playerRect);


		current_frame += current_animation->delta;
		current_animation->current_frame = current_frame;

	}

	PLAYER() : ENTITY() {
		loadAnims();
		playerRect->x = 595;
		playerRect->y = 214;
	}
};

class NPC : public ENTITY {

public:
	NPC() : ENTITY() {
	}
};

class NPC_JAMIE : public NPC {

	PLAYER target;

	bool targetIsRightOf() {
		return (target.playerRect->x > this->playerRect->x);
	}

	bool targetIsLeftOf() {
		return (target.playerRect->x < this->playerRect->x);
	}

	int lastPositionOf = 0;
public:
	SDL_Surface* winSurface;
	void scanTarget() {
		collider.setCollider(*playerRect, 1, 2, winSurface);

		if (targetIsLeftOf()==true) {
			if (lastPositionOf != 1) {
				//printf("Left of target!\n");
				lastPositionOf = 1;
			}
		}else{
			if (lastPositionOf != 0) {
				printf("Right of target!\n");
				lastPositionOf = 0;
			}
		}
	}

	void setTarget(PLAYER pl) {
		target = pl;
	}

	NPC_JAMIE(SDL_Surface* windowSurfaceRef = NULL) : NPC() {
		max_frames = 7.0f;
		playerSurface = SDL_LoadBMP("jamie.bmp");

		if (playerSurface == NULL) {
			printf("SDL Error: %s", SDL_GetError());
		}

		if (windowSurfaceRef != NULL) {
			winSurface = windowSurfaceRef;
		}

		playerRect->w = 75;
		playerRect->h = 89;
		playerRect->x = 225;
		playerRect->y = 197;

		addAnimation(animclips, 0, 10, 75, 89);
		playerSurface->format->Amask = 0xFe000000;
		playerSurface->format->Ashift = 24;
		Uint32 color = SDL_MapRGB(playerSurface->format, 0xff, 0x00, 0x00);
		SDL_SetColorKey(playerSurface, color, SDL_MapRGB(playerSurface->format, 0xff, 0x0, 0x00));

		this->deltaTime = 0.03f;
		
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

public:
	std::map<std::string, SPRITE_ANIMATION> anims;
	SPRITE_ANIMATION* current_animation;
	std::string MOB_NAME;
	int MOB_ID;
	ENTITY_LIFE Life;
	PLAYER target;

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
		return (target.playerRect->x > this->playerRect->x);
	}

	bool targetIsLeftOf() {
		return (target.playerRect->x < this->playerRect->x);
	}
	void scanTarget(std::string entity_name = "entity") {
		collider.setCollider(*playerRect, 1, 2, winSurface);

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

	void setTarget(PLAYER pl) {
		target = pl;
	}
	int lastPositionOf = 0;

	MOB_ENTITY() : ENTITY() {
		//loadAnims();
		//playerRect->x = 395;
		//playerRect->y = 214;
	}
};

void test() {
	int x;
}

class GAME {

public:
	std::map<std::string, MOB_ENTITY> MOBS;
	std::map<int, std::string> MOBS_LIST;


	void displayAllMobs(SDL_Surface* windowSurface, PLAYER target) {
		for (size_t i = 0; i < MOBS_LIST.size(); i++) {
			
			MOB_ENTITY* tmpMob = &MOBS[MOBS_LIST[i]];
			SPRITE_ANIMATION *mob_anim;
			mob_anim = tmpMob->current_animation;

			if (mob_anim->current_frame >= mob_anim->max_frames - 1) {
				mob_anim->current_frame = 0;
			}

			if (mob_anim->active == false) {
				mob_anim->current_frame = 0;
			}

			SDL_BlitSurface(mob_anim->display_surface, &mob_anim->animRects[static_cast<int>(mob_anim->current_frame)], windowSurface, tmpMob->playerRect);
			mob_anim->current_frame = mob_anim->current_frame + mob_anim->delta;

			tmpMob->setTarget(target);
			tmpMob->scanTarget(tmpMob->MOB_NAME);

			if (tmpMob->collider.findCollision(target.playerRect, tmpMob->MOB_NAME, NULL, NULL) == true) {
				tmpMob->state = attack;
			}else{
				tmpMob->state = idle;
			}
		}
	}

	void loadMobList() {
		tinyxml2::XMLDocument doc;
		tinyxml2::XMLElement* pRoot;
		doc.LoadFile("data\\mobs\\mobs.zenx");

		pRoot = doc.FirstChildElement("mobs")->FirstChildElement("mob");

		for (; pRoot != nullptr; pRoot = pRoot->NextSiblingElement("mob")) {
			std::string mob_name = pRoot->Attribute("name");
			int mob_id = pRoot->IntAttribute("id");
			int mob_init_health = pRoot->IntAttribute("Health");
			int mob_init_armour = pRoot->IntAttribute("Armour");
			int mob_init_mana = pRoot->IntAttribute("Mana");

			MOBS[mob_name].MOB_NAME = mob_name;
			MOBS[mob_name].MOB_ID = mob_id;
			MOBS[mob_name].Life.Health = mob_init_health;
			MOBS[mob_name].Life.Armour = mob_init_armour;
			MOBS[mob_name].Life.Mana = mob_init_mana;

			MOBS_LIST[MOBS_LIST.size()] = mob_name;

			tinyxml2::XMLElement* aRoot = pRoot->FirstChildElement("animations")->FirstChildElement("anim");

			for (; aRoot != nullptr; aRoot = aRoot->NextSiblingElement("anim")) {
				std::string sprite_anim_name = aRoot->Attribute("name");
				std::string sprite_filepath = aRoot->Attribute("sprite");
				float sprite_delta = aRoot->FloatAttribute("delta");
				float sprite_max_frames = aRoot->FloatAttribute("max_frames");
				int sprite_width = aRoot->IntAttribute("sprite_width");
				int sprite_height = aRoot->IntAttribute("sprite_height");

				MOBS[mob_name].anims[sprite_anim_name.c_str()].display_surface = NULL;
				MOBS[mob_name].anims[sprite_anim_name.c_str()].display_surface = IMG_Load(sprite_filepath.c_str());
				MOBS[mob_name].anims[sprite_anim_name.c_str()].max_frames = sprite_max_frames;
				MOBS[mob_name].anims[sprite_anim_name.c_str()].delta = sprite_delta;

				if (MOBS[mob_name].anims[sprite_anim_name.c_str()].display_surface == NULL) {
					printf("SDL Error: %s", SDL_GetError());
				}
				else {
					int tmp_f = static_cast<int>(sprite_max_frames);
					MOBS[mob_name].addAnimation(&MOBS[mob_name].anims[sprite_anim_name.c_str()], 0, tmp_f, sprite_width, sprite_height);
					MOBS[mob_name].playerRect->y = 220;
					MOBS[mob_name].playerRect->x = 414;
				}

				try {
					std::string tmpAnimName = pRoot->FirstChildElement("animations")->FirstChildElement("default_state")->Attribute("name");
					MOBS[mob_name].current_animation = &MOBS[mob_name].anims[tmpAnimName];
				}
				catch (...) {
					printf("Error setting default animation! Assuming idle_left has been added!\n");
				}
			}
			tinyxml2::XMLElement* anims = pRoot->FirstChildElement("anim");
		}
	}
	GAME() {

	}
};