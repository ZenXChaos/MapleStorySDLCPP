

class SPRITE_ANIMATION {

public:
	float max_frames;
	float current_frame = 0.0f;
	float delta;
	int yfactorup = 0;
	int yfactordown = 0;

	bool active = true;

	SDL_Surface* display_surface;
	SDL_Texture* sprite;
	SDL_Rect animRects[100];

	void addAnimation(int row, int cnt, int w, int h)
	{
		for (int i = 0; i < cnt; i += 1) {
			this->animRects[i].x = 0 + i * w;
			this->animRects[i].y = row * h;
			this->animRects[i].w = w;
			this->animRects[i].h = h;
		}
	}

	SDL_Texture* loadTexture(std::string path, SDL_Renderer* gRenderer)
	{
		//The final texture
		SDL_Texture* newTexture = NULL;

		//Load image at specified path
		SDL_Surface* loadedSurface = IMG_Load(path.c_str());
		if (loadedSurface == NULL)
		{
			printf("Unable to load image %s! SDL_image Error: %s\n", path.c_str(), IMG_GetError());
		}
		else
		{
			//Create texture from surface pixels
			sprite = SDL_CreateTextureFromSurface(gRenderer, loadedSurface);
			if (sprite == NULL)
			{
				printf("Unable to create texture from %s! SDL Error: %s\n", path.c_str(), SDL_GetError());
			}

			//Get rid of old loaded surface
			SDL_FreeSurface(loadedSurface);
		}

		return sprite;
	}

	SPRITE_ANIMATION() {
	}
};