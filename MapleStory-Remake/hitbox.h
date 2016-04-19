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

		//colliderFrame->clip_rect = pos;
		/* Declaring the surface. */
		SDL_Surface *s;

		/* Creating the surface. */
		s = SDL_CreateRGBSurface(0, spacingx*2 + pos.w, pos.h+(spacingy*2), 32, 0, 0, 0, 0);

		/* Filling the surface with red color. */
		SDL_FillRect(s, NULL, SDL_MapRGBA(s->format, 255, 0, 0, 255));

		SDL_BlitSurface(s, NULL, windowSurface, &pos);
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

	bool isCollidedXY(SDL_Rect collider_pos) {
		if (isCollidedX(collider_pos) == true && isCollidedY(collider_pos) == true) {
			if (lastCollide == 0) {
				printf("Collision XY!\n");
				lastCollide = 1;
			}
			return true;
		}
		else {
			if (lastCollide == 1) {
				printf("CollisionXY Lost!\n");
				lastCollide = 0;
			}

			return false;
		}
	}

	void findCollision(SDL_Rect *target) {
		SDL_Rect trgt = *target;

		isCollidedX(trgt);
		isCollidedY(trgt);
		isCollidedXY(trgt);
	}
};