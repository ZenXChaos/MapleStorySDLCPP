
class MAP {

protected:


public:

	SDL_Surface* mapSurface = nullptr;
	SDL_Rect* mapRect = new SDL_Rect();
	SPRITE_ANIMATION sprite;
	SDL_Renderer* gRenderer;

	void displayMap(SDL_Surface* windowSurface) {
		SDL_BlitSurface(mapSurface, mapRect, windowSurface, mapRect);
		SDL_RenderCopy(gRenderer, sprite.sprite, NULL, NULL);


	}

	MAP(SDL_Renderer* renderer = NULL) {
		if (renderer != NULL) {
			gRenderer = renderer;
		}
		mapRect->x = 0;
		mapRect->y = 0;
		mapRect->w = 679;
		mapRect->h = 376;

		sprite.loadTexture("maps\\start.png", gRenderer);
		//mapSurface = IMG_Load();
	}
};