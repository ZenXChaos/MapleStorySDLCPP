#pragma once

#include "hitbox.h"

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

	int state_trans = 0;
	int attack_statetrans = 0;

	int kbFactor = 0;
	int kbUpFactor = 0;
	int kbUpState = 0;
	float kbRecover = 0.0f;
	SDL_Rect kbTmpPos;
	std::vector<MOB_ENTITY>* spawned;
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
			anims[sprite_anim_name.c_str()].yfactorup = pRoot->IntAttribute("yfactorup");
			anims[sprite_anim_name.c_str()].yfactordown = pRoot->IntAttribute("yfactordown");

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

public:

	SKILL* sk;
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
			else if (this->KeyboardInput->isKeyHeld(event, SDL_SCANCODE_C)) {
				//playerRect->y -= 1;
				if (state != attack && spawned->size()>0) {
					sk->animation.current_frame = 0;
					attack_statetrans = 0;
					state = attack;
					if (FaceDirection == 0) {
						current_animation = &anims["attack01"];
					}
					else {

						current_animation = &anims["attack01_right"];
					}
					current_animation->current_frame = 0;
					current_frame = 0;
					std::random_device rd;     // only used once to initialise (seed) engine
					std::mt19937 rng(rd());    // random-number engine used (Mersenne-Twister in this case)
					std::uniform_int_distribution<int> uni(0, spawned->size()-1); // guaranteed unbiased

					int random_integer = uni(rng);
					sk->pos.x = spawned->at(random_integer).playerRect.x-30;
					sk->pos.y = spawned->at(random_integer).playerRect.y-183;
					
				}
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

	void playAnimation() {
		if (current_frame >= current_animation->max_frames-1) {
			current_frame = 0;
			if (state == attack) {
				if (FaceDirection == 0) {
					current_animation = &anims["idle_left"];
				}else{
					current_animation = &anims["idle_right"];
				}
				state = idle;
			}
		}
		if (sk->animation.current_frame >= sk->animation.max_frames - 1) {
			sk->animation.current_frame = -1;
		}
		//SDL_BlitSurface(current_animation->display_surface, &current_animation->animRects[static_cast<int>(current_frame)], windowSurface, playerRect);
		SDL_Rect tmpPlayerPos;
		tmpPlayerPos = playerRect;
		tmpPlayerPos.y = playerRect.y + current_animation->yfactordown - current_animation->yfactorup;
		//SDL_BlitSurface(mob_anim->display_surface, &mob_anim->animRects[static_cast<int>(mob_anim->current_frame)], windowSurface, &tmpMobPos);
		tmpPlayerPos.h = current_animation->animRects[static_cast<int>(current_animation->current_frame)].h;
		tmpPlayerPos.w = current_animation->animRects[static_cast<int>(current_animation->current_frame)].w;


		SDL_RenderCopy(gRenderer, current_animation->sprite, &current_animation->animRects[static_cast<int>(current_animation->current_frame)], &tmpPlayerPos);
		
		if (sk->animation.current_frame >= 0) {
			SDL_RenderCopy(gRenderer, sk->animation.sprite, &sk->animation.animRects[static_cast<int>(sk->animation.current_frame)], &sk->pos);
			sk->animation.current_frame += sk->animation.delta;
		}

		current_frame += current_animation->delta;
		current_animation->current_frame = current_frame;

	}

	PLAYER(SDL_Renderer* renderer = NULL, std::vector<MOB_ENTITY>* spwnd = NULL) : ENTITY() {
		if (renderer != NULL) {
			gRenderer = renderer;
		}
		this->spawned = spwnd;
		sk = new SKILL(gRenderer);
		loadAnims();
		playerRect.x = 595;
		playerRect.y = 214;
	}
};