

class SPRITE_ANIMATION {

public:
	float max_frames;
	float current_frame = 0.0f;
	float delta;
	int yfactorup = 0;
	int yfactordown = 0;

	bool active = true;

	SDL_Surface* display_surface;
	SDL_Rect animRects[100];

	SPRITE_ANIMATION() {
	}
};