#include <map>
#include <vector>
#include <SDL.h>


using namespace std;
#include "input.h"

void KeyInput::keyDownEvent(const SDL_Event &event, Uint32 ticks) {
	(*this->held_keys_)[event.key.keysym.scancode] = { true, ticks };
}

void KeyInput::keyPressed(const SDL_Event &event, Uint32 ticks) {

	this->keyDownEvent(event, ticks);
}

void KeyInput::keyReleased(const SDL_Event &event, Uint32 ticks) {
	this->keyUpEvent(event, ticks);

}

void KeyInput::keyUpEvent(const SDL_Event &event, Uint32 ticks) {

	(*this->held_keys_)[event.key.keysym.scancode] = { false, ticks };
}

bool KeyInput::isKeyHeld(const SDL_Event &event, SDL_Scancode scode) {
	std::pair<bool, Uint32> pair = (*this->held_keys_)[scode];
	return pair.first;
}