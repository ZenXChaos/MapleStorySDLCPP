
#include <SDL.h>
#include <iostream>
#include <SDL_EVENTS.h>

#include "core.h"


#undef main


int main(int argc, const char* argv[]) {
	SDL_Init(SDL_INIT_EVERYTHING);

	SDL_Window* window;
	SDL_Surface* surface;

	window = SDL_CreateWindow("Hi", 100, 100, 300, 300, SDL_WINDOW_RESIZABLE);
	surface = SDL_GetWindowSurface(window);

	SDL_Surface* color_surface = surface;

	SDL_FillRect(surface, NULL, COLOR_WHITE);

	bool b[4] = { 0,0,0,0 };
	SDL_Rect rect;
	float frame = 0;
	rect.x = 10;
	rect.y = 10;
	rect.w = 20;
	rect.h = 20;
	Uint32 color = SDL_MapRGB(surface->format, 0xff, 0xff, 0xff);
	Uint32 color2 = SDL_MapRGB(surface->format, 0, 0, 0);
	//image = SDL_LoadBMP("character.bmp");   // better to check later if image is NULL
	//SDL_Rect rects[10];
	//setrects(rects);
	//SDL_SetColorKey(image, SDL_SRCCOLORKEY, SDL_MapRGB(surface->format, 0x00, 0xff, 0xff));

	PLAYER character(COLOR_RED, 10, 10);
	character.setImage("character.bmp");
	character.SetColorKey(surface);

	Uint32 tick;
	bool running = true;
	while (running) {
		tick = SDL_GetTicks();
		SDL_Event event;
		while (SDL_PollEvent(&event)) {
			switch (event.type) {
				case SDL_QUIT:
					running = false;
					break;
				case SDL_KEYDOWN:
					switch (event.key.keysym.sym) {
					case SDLK_UP:
						b[0] = 1;
						character.player_state = walk_front;

						character.states.walking = true;
						break;
					case SDLK_LEFT:
						b[1] = 1;
						character.player_state = walk_left;

						character.states.walking = true;
						break;
					case SDLK_DOWN:
						b[2] = 1;
						character.player_state = walk_back;

						character.states.walking = true;
						break;
					case SDLK_RIGHT:

						b[3] = 1;
						character.player_state = walk_right;
						break;

					}

					character.states.walking = true;
					break;
				case SDL_KEYUP:
					switch (event.key.keysym.sym) {
					case SDLK_UP:
						b[0] = 0;
						character.displayPlayer(frame, surface);
						if (character.player_state == walk_front) {
							character.player_state = idle_front;
						}
						
						character.states.walking = false;
						break;
					case SDLK_LEFT:
						b[1] = 0;

						if (character.player_state == walk_left) {
							character.player_state = idle_left;
						}
						character.states.walking = false;
						break;
					case SDLK_DOWN:
						b[2] = 0;

						character.moveFix = false; 
						
						if (character.player_state == walk_back) {
							character.player_state = idle_back;
						}
						character.states.walking = false;
						break;
					case SDLK_RIGHT:
						b[3] = 0;

						if (character.player_state == walk_right) {
							character.player_state = idle_right;
						}
						character.states.walking = false;
						break;

				}
				break;
			}

		}

		switch (character.player_state) {
		case walk_left:

			character.ppos.x--;
			break;
		case walk_right:

			character.ppos.x++;
			break;
		case walk_back:

			character.ppos.y++;
			break;
		case walk_front:

			character.ppos.y--;
			break;
		}
		//logic
		if (b[0])
			rect.y--;
		if (b[1])
			rect.x--;
		if (b[2])
			rect.y++;
		if (b[3])
			rect.x++;
		SDL_FillRect(surface, &surface->clip_rect, color);
						
		character.displayPlayer(frame, surface);
		frame += 0.2f * character.animation_scrub;
		if (frame > 6) {
			frame = 0;
		}

		SDL_UpdateWindowSurface(window);
		limit_framerate(tick);
	}

	/*SPRITE character_sprite(COLOR_RED, 100, 100);
	SPRITE character_sprite2(COLOR_GREEN, 200, 200);

	SPRITE_GROUP main_sprites;

	character.setImage("assets\\characters\\01\\standing_01\\0.bmp");
	main_sprites.add(&character);
	main_sprites.add(&character_sprite2);
	main_sprites.remove(character_sprite);
	main_sprites.draw(surface);

	SDL_UpdateWindowSurface(window);


	int last_winpos_x, last_winpos_y;

	if (window == nullptr) {
		cout << "Failed to to fully initialize SDL!:\n" << SDL_GetError();
	}

	SDL_Event event;

	bool appOpen = true;

	SDL_Delay(3000);

	character.updateState_Next();
	character.update();
	SDL_UpdateWindowSurface(window);

	while (appOpen) {

		tick = SDL_GetTicks();

		while (SDL_PollEvent(&event)) {
			if (event.type == SDL_QUIT) {
				appOpen = false;
				
			}

			//CAP FPS at MAX_FRAMERATE (varaible `fps`)
			limit_framerate(tick);
			
			SDL_GetWindowPosition(window, &last_winpos_x, &last_winpos_y);
		}

		SDL_UpdateWindowSurface(window);
	}*/

	SDL_DestroyWindow(window);
	return 0;
}