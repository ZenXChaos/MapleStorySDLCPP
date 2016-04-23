#pragma once

#include "hitbox.h"
#include "input.h"

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
	SDL_Rect playerRect;
	HITBOX collider;
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

class ENTITY_LOGIC {
	ENTITY entity;
public:
	ENTITY_LOGIC() {

	}
};

class PLAYER : public ENTITY {
	std::map<std::string, SPRITE_ANIMATION> anims;
	SPRITE_ANIMATION* current_animation;
	SDL_Renderer* gRenderer = NULL;

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
			//anims[sprite_anim_name.c_str()].display_surface = IMG_Load(sprite_filepath.c_str());
			anims[sprite_anim_name.c_str()].loadTexture(sprite_filepath.c_str(), gRenderer);
			anims[sprite_anim_name.c_str()].max_frames = sprite_max_frames;
			anims[sprite_anim_name.c_str()].delta = sprite_delta;

			if (anims[sprite_anim_name.c_str()].sprite == NULL) {
				printf("SDL Error: %s", SDL_GetError());
			}
			else {
				int tmp_f = static_cast<int>(sprite_max_frames);
				addAnimation(&anims[sprite_anim_name.c_str()], 0, tmp_f, sprite_width, sprite_height);
				playerRect.y = 220;
				playerRect.x = 414;
				playerRect.w = sprite_width;
				playerRect.h = sprite_height;
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

	int kbFactor = 0;
	int kbUpFactor = 0;
	int kbUpState = 0;
	float kbRecover = 0.0f;
	SDL_Rect kbTmpPos;
public:

	void KnockBack() {
		if (kbRecover > 0.0f) {
			return;
		}
		if (kbUpState == 0) {
			for (kbTmpPos.y = 0; kbTmpPos.y < playerRect.y; kbTmpPos.y++) {
			}
			kbRecover = 5.0f;
			kbUpState = 1;
		}
		kbFactor = 100;
		kbUpFactor = 10;
	}

	void DamagePlayer() {

	}

	void playerMotorize(const SDL_Event & event) {
		if (kbRecover > 0) {
			kbRecover -= current_animation->delta;
		}
		else {
			kbRecover = 0;
		}
		if (kbFactor > 0 || kbUpFactor > 0){
			if (kbUpFactor > 5) {
				this->playerRect.y-=1;
			}else{
				this->playerRect.y+=1;
			}
			this->playerRect.x -= 100/15;
			kbFactor-= 100/10;
			
			kbUpFactor-= 2;
			kbUpState = 1;
		}
		else {
			if (kbUpState == 1) {
				kbUpState = 0;
				playerRect.y = kbTmpPos.y;
			}
			if (this->KeyboardInput->isKeyHeld(event, SDL_SCANCODE_LEFT)) {
				playerRect.x -= 2;
				state = walking;
				if (state_trans != 1) {
					current_animation = &anims["walk_left"];
					state_trans = 1;
					current_frame = 0;
				}
				FaceDirection = 0;
			}
			else if (this->KeyboardInput->isKeyHeld(event, SDL_SCANCODE_RIGHT)) {
				playerRect.x += 2;
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
		//SDL_BlitSurface(current_animation->display_surface, &current_animation->animRects[static_cast<int>(current_frame)], windowSurface, playerRect);
		SDL_Rect tmpPlayerPos;
		tmpPlayerPos = playerRect;
		tmpPlayerPos.y = playerRect.y;
		//SDL_BlitSurface(mob_anim->display_surface, &mob_anim->animRects[static_cast<int>(mob_anim->current_frame)], windowSurface, &tmpMobPos);
		tmpPlayerPos.h = current_animation->animRects[static_cast<int>(current_animation->current_frame)].h;
		tmpPlayerPos.w = current_animation->animRects[static_cast<int>(current_animation->current_frame)].w;

		SDL_RenderCopy(gRenderer, current_animation->sprite, &current_animation->animRects[static_cast<int>(current_animation->current_frame)], &tmpPlayerPos);
		
		current_frame += current_animation->delta;
		current_animation->current_frame = current_frame;

	}

	PLAYER(SDL_Renderer* renderer = NULL) : ENTITY() {
		if (renderer != NULL) {
			gRenderer = renderer;
		}
		loadAnims();
		playerRect.x = 595;
		playerRect.y = 214;
	}
};