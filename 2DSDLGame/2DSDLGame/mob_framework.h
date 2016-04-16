#pragma once

class MOB_ANIM {
public:
	SDL_Rect *frames = new SDL_Rect();
	int max_frames = 0;
};

class MOB : public PLAYER {

public:

	MOB_ANIM anim_idle_left;

	void addAnimation(SDL_Rect* clip, int row, int cnt, int w, int h)
	{
		for (int i = 0; i < cnt; i += 1) {
			clip[i].x = 0 + i * w;
			clip[i].y = row * h;
			clip[i].w = w;
			clip[i].h = h;
		}
	}


	void displayPlayer(float frame, SDL_Surface*surface) {
		switch (player_state)
		{

		default:
		case idle_front:
			//SDL_BlitSurface(sprite_image, &sprites_idle_left[static_cast<int>(frame)], surface, &ppos);
			break;

		case idle_left:
			if (frame <= anim_idle_left.max_frames-1) {
				SDL_BlitSurface(sprite_image, &anim_idle_left.frames[static_cast<int>(frame)], surface, &ppos);
			}
			else {
				SDL_BlitSurface(sprite_image, &anim_idle_left.frames[anim_idle_left.max_frames-1], surface, &ppos);
			}
			
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
	}

	MOB(Uint32 color, int x, int y, int sprite_width = 119, int sprite_height = 110) : PLAYER(color, x, y, sprite_width, sprite_height) {
		anim_idle_left.max_frames = 1;
		addAnimation(anim_idle_left.frames, 0, 1, 119, 110);

		this->animation_scrub = 0.0f;
		this->player_state = idle_left;
	}
};