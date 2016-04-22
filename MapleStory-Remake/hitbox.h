#define DEBUG_COLLIDER
class HITBOX {

	int lastCollide = 0;
public:
	int x1, x2;
	int y1, y2;

	void setCollider(SDL_Rect pos, int spacingx, int spacingy, SDL_Surface* windowSurface) {
		x1 = pos.x - spacingx;
		x2 = pos.x + spacingx;
		y1 = pos.y + spacingy;
		y2 = pos.y - spacingy;

		x1 = x1 - (pos.w / 2);
		x2 = x2 + (pos.w / 2);

		y1 = y1 - (pos.h / 2);
		y2 = y2 + (pos.h / 2);

		spacingy = spacingy*2;
		pos.x = x1  + (pos.w/2);
		pos.y -= spacingy;
	}

	bool isCollidedX(SDL_Rect collider_pos){
		bool collideX = false;
		if (collider_pos.x > x1 - (collider_pos.w / 4) && collider_pos.x < x2 + (collider_pos.w / 4)) {
			collideX = true;
			//printf("Collision X!\n");
		}

		return collideX;
	}

	bool isCollidedY(SDL_Rect collider_pos) {
		bool collideY = false;
		if (collider_pos.y > y1 - (collider_pos.h / 4) && collider_pos.y < y2 + (collider_pos.h / 4)) {
			collideY = true;
			//printf("Collision Y!\n");
		}

		return collideY;
	}

	bool isCollidedXY(SDL_Rect collider_pos, std::string entity_name = "entity", void(*callback_collision_enter)(void) = NULL, void(*callback_collision_exit)(void) = NULL) {
		if (isCollidedX(collider_pos) == true && isCollidedY(collider_pos) == true) {
			if (lastCollide != 1) {
				printf("Collision XY (%s) !\n", entity_name.c_str());
				if (callback_collision_enter != NULL) {
					callback_collision_enter();
				}
				lastCollide = 1;
			}
			return true;
		}
		else {
			if (lastCollide != 0) {
				printf("CollisionXY Lost!\n");
				if (callback_collision_exit != NULL) {
					callback_collision_exit();
				}
				lastCollide = 0;
			}

			return false;
		}
	}

	bool findCollision(SDL_Rect *target, std::string entity_name = "entity", void(*callback_collision_enter)(void) = NULL, void(*callback_collision_exit)(void) = NULL) {
		SDL_Rect trgt = *target;

		isCollidedX(trgt);
		isCollidedY(trgt);

		return isCollidedXY(trgt, entity_name);
	}
};