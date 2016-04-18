#pragma once
#ifndef INPUT_H
#define INPUT_H
#endif

#include <map>
#include <vector>

using namespace std;


namespace SDL2DGame {
	public ref class KeyInput {
		std::map<SDL_Scancode, std::pair<bool,Uint32>>* held_keys_ = new std::map<SDL_Scancode, std::pair<bool, Uint32>>();
		//std::map < SDL_Scancode, std::vector<bool, float> > * held_keys_2 = new std::map < SDL_Scancode, std::vector<bool, float> >();

		std::map<SDL_Scancode, std::pair<bool, float>>* held_keys_2 = new std::map<SDL_Scancode, std::pair<bool, float>>();
		std::map<SDL_Scancode, bool>* pressed_keys_ = new std::map<SDL_Scancode, bool>();
		std::map<SDL_Scancode, bool>* released_keys_ = new std::map<SDL_Scancode, bool>();


	public:
		void keyDownEvent(const SDL_Event &event, Uint32 ticks);
		void keyUpEvent(const SDL_Event &event, Uint32 ticks);

		void keyPressed(const SDL_Event &event, Uint32 ticks);
		void keyReleased(const SDL_Event &event, Uint32 ticks);
		bool isKeyHeld(const SDL_Event &event, SDL_Scancode scode);

		KeyInput() {

		}
	};
}