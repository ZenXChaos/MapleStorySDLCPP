#pragma once
class SPRITE {
private:
protected:
	SDL_Surface * sprite_image;
	SDL_Rect  sprite_rect;

public:
	int origin_x, origin_y;
	SPRITE(Uint32 color, int x, int y, int sprite_width = 46, int sprite_height = 78) {
		//Create Surface
		sprite_image = SDL_CreateRGBSurface(0, sprite_width, sprite_height, 32, 0, 0, 0, 0);

		//FILL Image
		SDL_FillRect(sprite_image, NULL, color);

		sprite_rect = sprite_image->clip_rect;

		sprite_rect.x = x - origin_x;
		sprite_rect.y = y - origin_y ;


	}
	void update() {

	}

	void draw(SDL_Surface * destination) {
		SDL_BlitSurface(sprite_image, NULL, destination, &sprite_rect);
	}

	SDL_Surface* getImage() const {
		return this->sprite_image;
	}

	bool operator==(const SPRITE &other) const {
		return this->sprite_image == other.getImage();
	};

};


class SPRITE_GROUP {
	
	vector<SPRITE*> sprites;
	size_t sprite_count;

public:
	SPRITE_GROUP() {
	}

	vector<SPRITE*> getSprites() {
		return this->sprites;
	}

	void add(SPRITE* added_sprite) {
		sprites.push_back(added_sprite);
		sprite_count = sprites.size();
	}

	void remove(SPRITE sprite_to_remove) {
		for (size_t i = 0; i < this->sprite_count; i++) {
			if (sprite_to_remove == *(this->sprites)[i]) {
				this->sprites.erase(sprites.begin()+i);
				this->sprite_count = this->sprites.size();
				return;
			}
		}
	}

	bool has(SPRITE sprite_to_lookfor) {
		for (size_t i = 0; i < this->sprite_count; i++) {
			if (sprite_to_lookfor == *(this->sprites)[i]) {

				//IS SAME SPRITE, RETURN TRUE
				return true;
			}
		}

		return false;
	}

	void update() {
		if (!this->sprites.empty()) {
			for (size_t i = 0; i < this->sprite_count; i++) {
				sprites[i]->update();
			}
		}

	}

	void clear() {
		if (!this->sprites.empty()) {
			for (size_t i = 0; i < this->sprite_count; i++) {
				sprites.clear();
			}
		}

	}

	int size() {
		return this->sprite_count;
	}

	void draw(SDL_Surface * destination) {
		if (!this->sprites.empty()) {
			for (size_t i = 0; i < this->sprite_count; i++) {
				sprites[i]->draw(destination);
			}
		}

	}

	SPRITE_GROUP copy() {
		SPRITE_GROUP newGroup;

		for (size_t i = 0; i < sprite_count; i++) {
			newGroup.add(this->sprites[i]);
		}

		return newGroup;
	}


	void operator+=(SPRITE *other) {
		add(other);
	}
};

class SPRITE_IMAGE : public SPRITE{

public:
	SPRITE_IMAGE(Uint32 color, int x, int y, int sprite_width = 46, int sprite_height = 78) : SPRITE(color, x, y, sprite_width, sprite_height){
		update_properties();
	}

	void set_position(int x, int y) {
		sprite_rect.x = origin_x - sprite_rect.x;
		sprite_rect.y = origin_y - sprite_rect.y;
	}

	void setImage(const char filename[] = NULL) {
		if (filename != NULL) {
			SDL_Surface* loadedImage = NULL;
			loadedImage = SDL_LoadBMP(filename);

			//ERR LOADING IMAGE?
			if (loadedImage == NULL) {
				return;
			}
			else {
				int last_posx, last_posy;
				int last_originx, last_originy;
				last_originx = origin_x;
				last_originy = origin_y;

				last_posx = sprite_image->clip_rect.x;
				last_posy = sprite_image->clip_rect.y;
				sprite_image = loadedImage;

				sprite_rect.x = last_posx;
				sprite_rect.y = last_posy;

				origin_x = last_originx;
				origin_y = last_originy;

				update_properties();
			}

		}

	}

	void SetColorKey(SDL_Surface*surface) {
		SDL_SetColorKey(sprite_image, SDL_SRCCOLORKEY, SDL_MapRGB(surface->format, 0x00, 0xff, 0xff));
	}

	void update_properties() {
		origin_x = 0;
		origin_y = 0;

		//set_position(sprite_rect.x, sprite_rect.y);
	}
};