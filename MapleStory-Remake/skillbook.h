
class SKILL {

public:
	SPRITE_ANIMATION animation;
	SDL_Renderer* renderer;
	SDL_Rect pos;


	SKILL(SDL_Renderer* gRenderer) {
		renderer = gRenderer;
		animation.loadTexture("player\\mage\\skill\\bolt01.png", gRenderer);
		animation.addAnimation(0, 10, 167, 297);
		animation.delta = 0.2f;
		animation.max_frames = 9.9f;
		animation.current_frame = -1.0f;
		pos.w = 167;
		pos.h = 297;

	}
};

class SKILLBOOK {

};