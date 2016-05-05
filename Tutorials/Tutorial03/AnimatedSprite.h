#ifndef ANIMATED_SPRITE_H
#define ANIMATED_SPRITE_H
class AnimatedSprite
{
	float frame = 0.0f;
public:
	float maxFrames = 0.9f;
	float delta = 0.01f;
	
	std::vector<Sprite> animFrames;

	AnimatedSprite();
	virtual ~AnimatedSprite();

	void AddSprite(Sprite sprite);
	void Step();
	void Animate(SDL_Rect pos, SDL_Renderer* gRenderer);
};

#endif