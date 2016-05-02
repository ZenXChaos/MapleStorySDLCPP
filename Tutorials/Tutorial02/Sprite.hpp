#ifndef SPRITE_H
#define  SPRITE_H
class Sprite
{

	SDL_Texture* texture = nullptr;
public:
	int w = 0;
	int h = 0;

	//Load texture from a file
	bool LoadTexture(std::string path, SDL_Renderer* gRenderer);

	//Draw texture to screen
	void Draw(SDL_Rect pos, SDL_Renderer* gRenderer);

	Sprite();
	virtual ~Sprite();
};
#endif