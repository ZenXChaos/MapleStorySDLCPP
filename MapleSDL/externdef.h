#ifndef EXTERNDEF_H
#define EXTERNDEF_H
extern class AnimatedSprite {
	float current_frame = 0.0f;
	float max_frames = 0.0f;
	float delta = 0.0f;
	SDL_Renderer* renderer;

public:
	SDL_RendererFlip flipType = SDL_FLIP_NONE;
	SDL_Texture* texture;
	SDL_Rect animclips[100];

	bool LoadTexture(std::string path, SDL_Renderer* gRenderer);
	void BuildAnimation(int row, int cnt, int w, int h, float d);
	void Animate(SDL_Rect pos, double angle, SDL_Point* center, SDL_RendererFlip flip);

	AnimatedSprite() {}

};
#endif