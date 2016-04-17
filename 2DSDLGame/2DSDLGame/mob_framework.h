#pragma once

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
			SDL_Delay(1000);
			skipFrame = true;
			return;
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

	void addAnimation(SDL_Rect* clip, int row, int cnt, int w, int h)
	{
		for (int i = 0; i < cnt; i += 1) {
			clip[i].x = 0 + i * w;
			clip[i].y = row * h;
			clip[i].w = w;
			clip[i].h = h;
		}
	}


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

		anim_idle_left->max_frames = 4;
		anim_idle_left->sprite_image = *SDL_LoadBMP("mobs\\shadewraith\\standing\\left.bmp");

		addAnimation(anim_idle_left->frames, 0, anim_idle_left->max_frames, sprite_width, sprite_height);

		this->animation_scrub = 0.0f;
		this->player_state = idle_left;
	}
};