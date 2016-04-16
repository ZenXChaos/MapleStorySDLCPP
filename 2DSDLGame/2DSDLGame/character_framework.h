#pragma once
enum PLAYER_STATE {

};

class PLAYER_ANIMATION_DATA {
public:
	string anim_play_name;
	string bmp_path;
};

class PLAYER_STATES {
public:
	bool walking = false;
};

class PLAYER : public SPRITE_IMAGE {


public:
	SDL_Rect player_pos[10];
	SDL_Rect sprites_idle_left[10];
	SDL_Rect sprites_idle_right[10];
	SDL_Rect sprites_idle_front[10];
	SDL_Rect sprites_idle_back[10];

	SDL_Rect sprites_walk_front[10];
	SDL_Rect sprites_walk_back[10];
	SDL_Rect sprites_walk_left[10];
	SDL_Rect sprites_walk_right[10];


	PLAYER_STATES states;

	void addAnimation(SDL_Rect* clip, int row, int cnt)
	{
		for (int i = 0; i < cnt; i += 1) {
			clip[i].x = 0 + i * 64;
			clip[i].y = row * 64;
			clip[i].w = 64;
			clip[i].h = 64;
		}
	}



	int player_state_index = 0;

	vector<PLAYER_ANIM*> anims;

	PLAYER(Uint32 color, int x, int y, int sprite_width = 46, int sprite_height = 78) : SPRITE_IMAGE(color, x, y, sprite_width, sprite_height) {
		//update_properties();
		for (int i = 0; i < 9; i += 1) {
			player_pos[i].h = 64;
			player_pos[i].w = 64;
			player_pos[i].x = 10;
			player_pos[i].y = 10;
		}
		

		addAnimation(sprites_idle_left, 2, 9);
		addAnimation(sprites_idle_right, 0, 6);
		addAnimation(sprites_idle_front, 3, 6);
		addAnimation(sprites_idle_back, 1, 6);

		addAnimation(sprites_walk_left, 11, 9);
		addAnimation(sprites_walk_right, 9, 9);
		addAnimation(sprites_walk_front, 8, 9);
		addAnimation(sprites_walk_back, 10, 9);
	}

	void displayPlayer(int frame, SDL_Surface*surface) {
		switch (player_state_index)
		{

		default:
		case 1:
			SDL_BlitSurface(sprite_image, &sprites_idle_front[static_cast<int>(frame)], surface, player_pos);
			break;

		case 0:
			SDL_BlitSurface(sprite_image, &sprites_idle_left[static_cast<int>(frame)], surface, player_pos);
			break;

		case 3:
			SDL_BlitSurface(sprite_image, &sprites_idle_right[static_cast<int>(frame)], surface, player_pos);
			break;

		case 2:
			SDL_BlitSurface(sprite_image, &sprites_idle_back[static_cast<int>(frame)], surface, player_pos);
			break;

		case 4:
			SDL_BlitSurface(sprite_image, &sprites_walk_front[static_cast<int>(frame)], surface, player_pos);
			break;

		case 5:
			SDL_BlitSurface(sprite_image, &sprites_walk_left[static_cast<int>(frame)], surface, player_pos);
			break;

		case 6:
			SDL_BlitSurface(sprite_image, &sprites_walk_right[static_cast<int>(frame)], surface, player_pos);
			break;

		case 7:
			SDL_BlitSurface(sprite_image, &sprites_walk_back[static_cast<int>(frame)], surface, player_pos);
			break;
		}
	}

};
