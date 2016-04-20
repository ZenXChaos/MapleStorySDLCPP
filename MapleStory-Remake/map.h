
class MAP {

protected:


public:

	SDL_Surface* mapSurface = nullptr;
	SDL_Rect* mapRect = new SDL_Rect();

	void displayMap(SDL_Surface* windowSurface) {
		SDL_BlitSurface(mapSurface, mapRect, windowSurface, mapRect);


	}

	MAP() {
		mapRect->x = 0;
		mapRect->y = 0;
		mapRect->w = 679;
		mapRect->h = 376;

		mapSurface = IMG_Load("maps\\start.png");
	}
};