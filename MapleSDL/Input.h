#ifndef INPUT_H
#define INPUT_H
class Input {
	std::map<SDL_Scancode, bool> keysDown;
	std::map<SDL_Scancode, bool> keysUp;
public:
	void KeyDown(SDL_Scancode key);
	void KeyUp(SDL_Scancode key);
	bool IsKeyPressed(SDL_Scancode key);

};
#endif