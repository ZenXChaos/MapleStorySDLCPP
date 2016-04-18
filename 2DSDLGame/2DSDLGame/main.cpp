

#include "core.h"


#undef main

namespace ZenXChaos {
	public ref class CHAOS sealed {
		CHAOS() {

		}
	};
}


int main(int argc, const char* argv[]) {
	_CrtSetDbgFlag(_CRTDBG_CHECK_ALWAYS_DF);
	SDL_Init(SDL_INIT_EVERYTHING);

	SDL2DGame::KeyInput^ KeyboardInput = gcnew SDL2DGame::KeyInput();

	SDL_Window* window;
	SDL_Surface* surface;
	SDL_Surface* surface2;

	window = SDL_CreateWindow("Hi", 100, 100, 300, 300, SDL_WINDOW_RESIZABLE);
	surface = SDL_GetWindowSurface(window);
	surface2 = SDL_GetWindowSurface(window);

	SDL_Surface* color_surface = surface;

	SDL_FillRect(surface, NULL, COLOR_WHITE);
	SDL_FillRect(surface2, NULL, COLOR_WHITE);

	bool b[4] = { 0,0,0,0 };
	float frame = 0;
	Uint32 color = SDL_MapRGB(surface->format, 0xff, 0xff, 0xff);
	Uint32 color2 = SDL_MapRGB(surface2->format, 0, 0, 0);
	//image = SDL_LoadBMP("character.bmp");   // better to check later if image is NULL
	//SDL_Rect rects[10];
	//setrects(rects);
	//SDL_SetColorKey(image, SDL_SRCCOLORKEY, SDL_MapRGB(surface->format, 0x00, 0xff, 0xff));

	PLAYER character(COLOR_RED, 10, 10);
	//MOB shadewraith(COLOR_RED, 116, 102, 116, 102);
	//shadewraith.setImage("mobs\\shadewraith\\standing\\left.bmp");
	//shadewraith.SetColorKey(surface2);

	character.setImage("character.bmp");
	character.SetColorKey(surface2);


	SDL2DGame::MOB_SHADEWRAITH^ shaderWraith = gcnew SDL2DGame::MOB_SHADEWRAITH(116,102, surface2);


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
					/*switch (event.key.keysym.sym) {
					case SDLK_UP:
						b[0] = 1;
						character.player_state = walk_front;

						character.states.walking = true;
						//shadewraith.setImage("character.bmp");
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

					character.states.walking = true;*/

					KeyboardInput->keyDownEvent(event, tick);
					break;
				case SDL_KEYUP:
					/*
					switch (event.key.keysym.sym) {
					case SDLK_UP:
						b[0] = 0;
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
					*/

					KeyboardInput->keyUpEvent(event, tick);
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

		SDL_FillRect(surface, &surface->clip_rect, color);
		SDL_FillRect(surface2, &surface2->clip_rect, color);

		shaderWraith->Draw();
		//shadewraith.displayPlayer(surface);
		character.displayPlayer(frame, surface, event, KeyboardInput);
		

		frame += 0.2f * character.animation_scrub;

		if (frame > 6) {
			frame = 0;
		}

		SDL_UpdateWindowSurface(window);
		limit_framerate(tick);
	}

	shaderWraith->Kill();
	SDL_FreeSurface(surface);
	SDL_FreeSurface(surface2);
	SDL_DestroyWindow(window);
	return 0;
}