#ifndef ANIMATEDSPRITE_H
#define ANIMATEDSPRITE_H
#include <SDL.h>

class AnimatedSprite {
	float current_frame = 0.0f;
	float max_frames = 0.0f;
	float delta = 0.0f;
	float percentDone = 0.0f;

	bool animFinished = false;

	SDL_Renderer* renderer = nullptr;

public:
	SDL_RendererFlip flipType = SDL_FLIP_NONE;
	SDL_Texture* texture = nullptr;
	SDL_Rect animclips [100];

	int yfactor = 0, xfactor = 0;

	float getDelta() {
		return this->delta;
	}
	float percentComplete() {
		return this->percentDone;
	}

	bool isFinishedPlaying() {
		return this->animFinished;
	}
	bool LoadTexture(std::string path, SDL_Renderer* gRenderer);
	void BuildAnimation(int row, int cnt, int w, int h, float d);
	void Animate(SDL_Rect pos, double angle, SDL_Point* center, SDL_RendererFlip flip, SDL_Rect* frameData = nullptr);

	SDL_Renderer* getRenderer() {
		return this->renderer;
	}
	AnimatedSprite(){}

};
#endif