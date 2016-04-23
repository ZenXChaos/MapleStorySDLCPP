class _HITBOX {
public:
	void* obj;

	SDL_Rect rect;
	int entity_type;
	int HITBOX_ID=0;


	std::map<int, bool> currentCollisions;
};

class AUTOHITBOX {

public:
	
	std::map<int, _HITBOX> rectBinds;

	std::map<std::string,std::vector<_HITBOX>> currentCollisions;

	int spacingX = 0;
	int spacingY = 0;

	AUTOHITBOX() {

	}

	bool busy = false;

	void bindBoxToRect(void* obj, int hid) {
		while (busy) {}
		PLAYER* p;
		MOB_ENTITY* me;
		busy = true;
		try {
			p = static_cast<PLAYER*>(obj);
			p->state = p->state;

			if (p->playerRect.w > 0) {

				_HITBOX hb;
				hb.entity_type = 0;
				hb.obj = obj;
				hb.rect = p->playerRect;
				rectBinds[rectBinds.size()] = hb;
				rectBinds[rectBinds.size() - 1].HITBOX_ID = hid;
			}
		}
		catch (...) {
			printf("Is not of type Player");
		}

		try {
			me = static_cast<MOB_ENTITY*>(obj);
			if (me->playerRect.w > 0) {

				rectBinds[rectBinds.size()].obj = obj;
				rectBinds[rectBinds.size()-1].rect = me->playerRect;
				rectBinds[rectBinds.size()-1].entity_type = 1;
				rectBinds[rectBinds.size() - 1].HITBOX_ID = hid;

			}
		}
		catch (...) {
			printf("Is not of type MOB_ENTITY");
		}

		busy = false;
		return;
	}

	void showHitbox(SDL_Renderer *gRenderer) {
		for (size_t i = 0; i < rectBinds.size(); i++) {
			//Render red filled quad
			SDL_Rect fillRect;// = { bindedRect->x - spacingX, bindedRect->y - spacingY, bindedRect->w + spacingX, bindedRect->h + spacingY };
			
			SDL_Rect bindedRect;
			bindedRect = rectBinds[i].rect;
			fillRect.x = bindedRect.x - spacingX;
			fillRect.y = bindedRect.y - spacingY;
			fillRect.w = bindedRect.w + spacingX*2;
			fillRect.h = bindedRect.h + spacingY;

			SDL_SetRenderDrawColor(gRenderer, 0xFF, 0x00, 0x00, 0xFF);
			//SDL_RenderFillRect(gRenderer, &fillRect);
			SDL_RenderDrawRect(gRenderer, &fillRect);
		}
	}

	bool isCollidedX(SDL_Rect collider_pos, SDL_Rect collider_pos2) {
		if (collider_pos.x > collider_pos2.x  && collider_pos.x < (collider_pos2.x+ collider_pos2.w)) {
			return true;
		}

		return false;
	}

	bool isCollidedY(SDL_Rect collider_pos, SDL_Rect collider_pos2) {
		if (collider_pos.y > collider_pos2.y - (collider_pos.h / 4) && collider_pos.y < (collider_pos2.y+ collider_pos2.h) + (collider_pos.h / 4)) {
			return true;
		}

		return false;
	}

	bool isCollidedXY(SDL_Rect collider_pos, SDL_Rect collider_pos2, std::string entity_name = "entity", void(*callback_collision_enter)(void) = NULL, void(*callback_collision_exit)(void) = NULL) {
		if (isCollidedX(collider_pos,collider_pos2) == true && isCollidedY(collider_pos, collider_pos2) == true) {
			return true;
		}
		else {
			return false;
		}
	}


	void checkCollision(PLAYER* pp, std::map<int, _HITBOX> _rectBinds) {
		for (size_t i = 0; i < _rectBinds.size(); i++) {
			for (size_t ii = 0; ii < _rectBinds.size(); ii++) {
				if (i != ii) {
					SDL_Rect pos1;
					SDL_Rect pos2;

					pos1 = _rectBinds[ii].rect;
					pos2 = _rectBinds[i].rect;

					pos1.x += spacingX;
					pos1.y += spacingY;

					if (isCollidedXY(pos1, pos2) == true) {
						//printf("Colliding! New Collision Engine!");
						if (_rectBinds[i].currentCollisions[_rectBinds[ii].HITBOX_ID] == false) {
							if (_rectBinds[i].entity_type == 0) {

								if (pp != nullptr) {
									char tmpString[255] = "";
									sprintf(tmpString, "ENTITY %i WITH ENTITY %i", _rectBinds[i].HITBOX_ID, _rectBinds[ii].HITBOX_ID);
									pp->sendMessage("hit", _rectBinds[ii].HITBOX_ID);
								}
							}
							else if (_rectBinds[ii].entity_type == 1) {
								
								MOB_ENTITY* me = static_cast<MOB_ENTITY*>(_rectBinds[ii].obj);
								if (me != nullptr) {
									char tmpString[255] = "";
									sprintf(tmpString, "ENTITY %i WITH ENTITY %i", _rectBinds[i].HITBOX_ID, _rectBinds[ii].HITBOX_ID);
									me->sendMessage(tmpString, _rectBinds[ii].HITBOX_ID);
								}
							}
							
						}
						_rectBinds[ii].currentCollisions[_rectBinds[i].HITBOX_ID] = true;
					}
					else
					{
						_rectBinds[ii].currentCollisions[_rectBinds[i].HITBOX_ID] = false;

					}
				}
			}
		}
	}

	void showHitboxes() {

	}
};