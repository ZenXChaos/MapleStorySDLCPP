#include <SDL.h>

#include "input.h"


void SDL2DGame::KeyInput::keyDownEvent(const SDL_Event &event, Uint32 ticks) {
	(*this->held_keys_)[event.key.keysym.scancode] = { true, ticks };
}

void SDL2DGame::KeyInput::keyPressed(const SDL_Event &event, Uint32 ticks) {
	
	this->keyDownEvent(event, ticks);
}

void SDL2DGame::KeyInput::keyReleased(const SDL_Event &event, Uint32 ticks){
	this->keyUpEvent(event, ticks);

}

void SDL2DGame::KeyInput::keyUpEvent(const SDL_Event &event, Uint32 ticks) {

	(*this->held_keys_)[event.key.keysym.scancode] = { false, ticks };
}

bool SDL2DGame::KeyInput::isKeyHeld(const SDL_Event &event, SDL_Scancode scode) {
	std::pair<bool,Uint32> pair = (*this->held_keys_)[scode];
	return pair.first;
}