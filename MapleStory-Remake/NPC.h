class NPC : public ENTITY {

public:
	NPC() : ENTITY() {
	}
};

class NPC_JAMIE : public NPC {

	PLAYER target;

	bool targetIsRightOf() {
		return (target.playerRect.x > this->playerRect.x);
	}

	bool targetIsLeftOf() {
		return (target.playerRect.x < this->playerRect.x);
	}

	int lastPositionOf = 0;
public:
	SDL_Surface* winSurface;
	void scanTarget() {
		collider.setCollider(playerRect, 1, 2, winSurface);

		if (targetIsLeftOf() == true) {
			if (lastPositionOf != 1) {
				//printf("Left of target!\n");
				lastPositionOf = 1;
			}
		}
		else {
			if (lastPositionOf != 0) {
				printf("Right of target!\n");
				lastPositionOf = 0;
			}
		}
	}

	void setTarget(PLAYER pl) {
		target = pl;
	}

	NPC_JAMIE(SDL_Surface* windowSurfaceRef = NULL) : NPC() {
		max_frames = 7.0f;
		playerSurface = SDL_LoadBMP("jamie.bmp");

		if (playerSurface == NULL) {
			printf("SDL Error: %s", SDL_GetError());
		}

		if (windowSurfaceRef != NULL) {
			winSurface = windowSurfaceRef;
		}

		playerRect.w = 75;
		playerRect.h = 89;
		playerRect.x = 225;
		playerRect.y = 197;

		addAnimation(animclips, 0, 10, 75, 89);
		playerSurface->format->Amask = 0xFe000000;
		playerSurface->format->Ashift = 24;
		Uint32 color = SDL_MapRGB(playerSurface->format, 0xff, 0x00, 0x00);
		SDL_SetColorKey(playerSurface, color, SDL_MapRGB(playerSurface->format, 0xff, 0x0, 0x00));

		this->deltaTime = 0.03f;

	}
};