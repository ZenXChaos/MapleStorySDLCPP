class Input {
	std::map<SDL_Scancode, bool> keysDown;
	std::map<SDL_Scancode, bool> keysUp;
public:
	void KeyDown(SDL_Scancode key);
	void KeyUp(SDL_Scancode key);
	bool IsKeyPressed(SDL_Scancode key);

};