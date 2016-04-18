#pragma once

using namespace System;
using namespace System::Collections::Generic;

using namespace cli;

namespace SDL2DGame {
	public ref class MOB_ANIMFRAME {
	public:
		MOB_ANIMFRAME() {

		}
	};

	public ref class MOB_ANIM {

	public:
		SDL_Rect *frames = new SDL_Rect();
		SDL_Surface*  sprite_image;
		float deltaTime;
		int max_frames;
		float current_frame;
		bool skipFrame = false;
		void play(SDL_Surface*surface, SDL_Rect pos) {
			SDL_Rect tmpPos = pos;
			if (skipFrame == false) {
				//SDL_Delay(1000);
				//skipFrame = true;
				//return;
			}
			if (this->current_frame >= this->max_frames) {
				this->current_frame = 0;
				SDL_BlitSurface(sprite_image, &this->frames[0], surface, &pos);
			}
			else {
				SDL_BlitSurface(sprite_image, &this->frames[static_cast<int>(this->current_frame)], surface, &pos);
			}

			this->current_frame += deltaTime;
		}

		MOB_ANIM() {

		}

	};

	public ref class MOB_ANIMSET {

		Dictionary<String^, MOB_ANIM^>^ anims = gcnew Dictionary<String^, MOB_ANIM^>();

	public:
		void addAnimation(String^ animName, SDL_Surface* sprite, int row, int cnt, int w, int h, float delta)
		{ 
			SDL_Rect* clip = new SDL_Rect();
				MOB_ANIM^ tmpMobAnim = gcnew MOB_ANIM();
			if (!delta || delta <= 0) {
				delta = 0.2f;
			}

			bool animFound = anims->ContainsKey(animName);
			if (animFound == false) {
				anims[animName] = gcnew MOB_ANIM();
				//anims[animName]->
				for (int i = 0; i < cnt; i += 1) {
					clip[i].x = 0 + i * w;
					clip[i].y = row * h;
					clip[i].w = w;
					clip[i].h = h;
				}

				tmpMobAnim = anims[animName];

				tmpMobAnim->frames = clip;
				tmpMobAnim->sprite_image = sprite;
				tmpMobAnim->max_frames = cnt;
				tmpMobAnim->deltaTime = delta;
			}
		}

		MOB_ANIMSET() {

		}

		property MOB_ANIM^ IdleAnimation_Left {
			MOB_ANIM^ get() {
				MOB_ANIM^ tmpMobAnim = this->anims["idle_left"];

				return tmpMobAnim;
			}
		}
	};

	public ref class MOB_LIFE{ 

		int health;
		int armour;
		int mana;

	public:

		MOB_LIFE(int h, int a, int m) {
			this->health = h;
			this->armour = a;
			this->mana = m;
		}

		property int Health {
			int get() {
				return this->health;
			}

			void set(int val) {
				this->health = val;
			}
		}

		property int Armour {
			int get() {
				return this->armour;
			}

			void set(int val) {
				this->armour = val;
			}
		}

		property int Mana {
			int get() {
				return this->mana;
			}

			void set(int val) {
				this->mana = val;
			}
		}
	};

	public ref class MOB {

	protected:
		SDL_Surface* surface;
		MOB_ANIMSET^ anim_idle = gcnew MOB_ANIMSET();
		MOB_ANIMSET^ anim_movement = gcnew MOB_ANIMSET();
		MOB_ANIMSET^ anim_attack = gcnew MOB_ANIMSET();
		MOB_LIFE^ life = gcnew MOB_LIFE(100, 10, 0);
		SDL_Rect* pos = new SDL_Rect();
		String^ mob_name;



	public:
		MOB_ANIM^ CurrentAnimationPlaying;

		void Draw() {
			if (CurrentAnimationPlaying != nullptr) {
				CurrentAnimationPlaying->play(surface, *pos);
			}
		}

		property MOB_ANIMSET^ IdleAnimations {
			MOB_ANIMSET^ get() {
				return this->anim_idle;
			}
		}

		MOB(int sprite_width, int sprite_height, SDL_Surface* refsurface){ 

			//Default player position
			this->pos->x = 0;
			this->pos->y = 10;
			this->pos->w = sprite_width;
			this->pos->h = sprite_height;

			this->surface = refsurface; 

		}
	};

	public ref class MOB_SHADEWRAITH : public MOB{

	public:
		void Kill() {
			if (this->anim_idle->IdleAnimation_Left != nullptr)
				SDL_FreeSurface(this->anim_idle->IdleAnimation_Left->sprite_image);
		}
		MOB_SHADEWRAITH(int sprite_width, int sprite_height, SDL_Surface* refsurface) : MOB(sprite_width, sprite_height, refsurface){


			this->anim_idle->addAnimation("idle_left", SDL_LoadBMP("mobs\\shadewraith\\standing\\left.bmp"), 0, 5, 116, 102, 0.3f);
			if (this->anim_idle->IdleAnimation_Left->sprite_image == NULL) {
				
			}else{
				this->CurrentAnimationPlaying = this->anim_idle->IdleAnimation_Left;
			}
		}
	};
}

class MOB_ANIM {
	float current_frame = 0.0f;
	bool skipFrame = false;
public:
	SDL_Rect *frames = new SDL_Rect();
	SDL_Surface  sprite_image;
	float deltaTime = 0.2f;
	int max_frames = 0;

	void play(SDL_Surface*surface, SDL_Rect* pos) {
		if (skipFrame == false) {
			//SDL_Delay(1000);
			//skipFrame = true;
			//return;
		}
		if (this->current_frame >= this->max_frames) {
			this->current_frame = 0;
			SDL_BlitSurface(&sprite_image, &this->frames[0], surface, pos);
			return;
		}
		SDL_BlitSurface(&sprite_image, &this->frames[static_cast<int>(this->current_frame)], surface, pos);

		this->current_frame += deltaTime;
	}
};

class MOB : public PLAYER {

public:

	MOB_ANIM* anim_idle_left = new MOB_ANIM();

	MOB_ANIM * CurrentAnimationPlaying;
	SDL_Rect *ppos = new SDL_Rect();



	void displayPlayer(SDL_Surface*surface) {
		switch (player_state)
		{

		default:
		case idle_front:
			//SDL_BlitSurface(sprite_image, &sprites_idle_left[static_cast<int>(frame)], surface, &ppos);
			this->CurrentAnimationPlaying = anim_idle_left;
			break;

		case idle_left:
			this->CurrentAnimationPlaying = anim_idle_left;

			
			break;

		case idle_right:
			//SDL_BlitSurface(sprite_image, &sprites_idle_right[static_cast<int>(frame)], surface, &ppos);
			break;

		case idle_back:
			//SDL_BlitSurface(sprite_image, &sprites_idle_back[static_cast<int>(frame)], surface, &ppos);
			break;

		case walk_front:
			//SDL_BlitSurface(sprite_image, &sprites_walk_front[static_cast<int>(frame)], surface, &ppos);
			break;

		case walk_left:
			//SDL_BlitSurface(sprite_image, &sprites_walk_left[static_cast<int>(frame)], surface, &ppos);
			break;

		case walk_right:
			//SDL_BlitSurface(sprite_image, &sprites_walk_right[static_cast<int>(frame)], surface, &ppos);
			break;

		case walk_back:
			//SDL_BlitSurface(sprite_image, &sprites_walk_back[static_cast<int>(frame)], surface, &ppos);
			break;
		}

		if (CurrentAnimationPlaying != nullptr) {
			CurrentAnimationPlaying->play(surface, ppos);
		}
	}

	MOB(Uint32 color, int x, int y, int sprite_width = 106, int sprite_height = 102) : PLAYER(color, x, y, sprite_width, sprite_height) {

		//Default player position
		this->ppos->x = 0;
		this->ppos->y = 10;
		this->ppos->w = sprite_width;
		this->ppos->h = sprite_height;

		anim_idle_left->max_frames = 5;
		anim_idle_left->sprite_image = *SDL_LoadBMP("mobs\\shadewraith\\standing\\left.bmp");

		addAnimation(anim_idle_left->frames, 0, anim_idle_left->max_frames, sprite_width, sprite_height);

		this->animation_scrub = 0.0f;
		this->player_state = idle_left;
	}
};