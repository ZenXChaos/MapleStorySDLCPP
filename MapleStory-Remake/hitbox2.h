class _HITBOX {
public:
	void* obj;

	int entity_type;
};

class AUTOHITBOX {

public:
	SDL_Rect* bindedRect;
	std::map<int, _HITBOX*> rectBinds;

	int spacingX = 0;
	int spacingY = 0;

	AUTOHITBOX(SDL_Rect* rect_bind) {
		bindedRect = rect_bind;
	}

	void bindBoxToRect(void* obj) {
		
		PLAYER* p;
		MOB_ENTITY* me;

		try {
			p = static_cast<PLAYER*>(obj);
			p->state = p->state;

		}
		catch (...) {
			printf("Is not of type Player");
		}

		try {
			me = static_cast<MOB_ENTITY*>(obj);
			if (&me->MOB_NAME != nullptr) {

				_HITBOX hb;
				hb.entity_type = 1;
				hb.obj = obj;
				rectBinds[rectBinds.size()] = &hb;

			}
		}
		catch (...) {
			printf("Is not of type MOB_ENTITY");
		}
	}

	void showHitbox(SDL_Renderer *gRenderer) {
		//Render red filled quad
		SDL_Rect fillRect;// = { bindedRect->x - spacingX, bindedRect->y - spacingY, bindedRect->w + spacingX, bindedRect->h + spacingY };
		fillRect.x = bindedRect->x - spacingX;
		fillRect.y = bindedRect->y - spacingY;
		fillRect.w = bindedRect->w + spacingX;
		fillRect.h = bindedRect->h + spacingY;
		SDL_SetRenderDrawColor(gRenderer, 0xFF, 0x00, 0x00, 0xFF);
		//SDL_RenderFillRect(gRenderer, &fillRect);
		SDL_RenderDrawRect(gRenderer, &fillRect);
	}

	void showHitboxes() {

	}
};