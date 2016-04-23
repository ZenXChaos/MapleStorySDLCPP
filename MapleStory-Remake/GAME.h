class GAME {

	int ready = 0;
public:
	std::map<std::string, MOB_ENTITY> MOBS;
	std::map<int, std::string> MOBS_LIST;
	SDL_Renderer* gRenderer = NULL;

	void displayAllMobs(PLAYER *target, AUTOHITBOX* ahb, bool pause = 0) {
		float cFrame = 0.0f;
		for (size_t i = 0; i < spawnmgr.tmpSize; i++) {


			MOB_ENTITY* tmpMob = &spawnmgr.spawned.at(i);
			SPRITE_ANIMATION *mob_anim;
			mob_anim = tmpMob->current_animation;
			cFrame = mob_anim->current_frame;

			for (size_t a = 1; a < ahb->rectBinds.size(); a++) {
				if (ahb->rectBinds.at(a).HITBOX_ID == tmpMob->ENTITY_ID) {
					ahb->rectBinds[a].rect = tmpMob->playerRect;
					ahb->rectBinds[a].obj = static_cast<void*>(tmpMob);
				}
			}

			if (pause == true) {
				cFrame = 0;
			}

			if (mob_anim->current_frame >= mob_anim->max_frames - 1 && cFrame > 0.0f) {
				mob_anim->current_frame = 0;
			}

			if (mob_anim->active == false && cFrame > 0.0f) {
				mob_anim->current_frame = 0;
			}

			SDL_Rect tmpMobPos;
			tmpMobPos = tmpMob->playerRect;
			tmpMobPos.y = (tmpMob->playerRect.y - mob_anim->yfactorup) + mob_anim->yfactordown;
			//SDL_BlitSurface(mob_anim->display_surface, &mob_anim->animRects[static_cast<int>(mob_anim->current_frame)], windowSurface, &tmpMobPos);
			tmpMobPos.h = mob_anim->animRects[static_cast<int>(mob_anim->current_frame)].h;
			tmpMobPos.w = mob_anim->animRects[static_cast<int>(mob_anim->current_frame)].w;


			SDL_SetTextureAlphaMod(mob_anim->sprite, static_cast<int>(tmpMob->Life.eop));
			if (cFrame == 0) {
				SDL_RenderCopy(gRenderer, mob_anim->sprite, &mob_anim->animRects[static_cast<int>(mob_anim->current_frame)], &tmpMobPos);
			}else{
				SDL_RenderCopy(gRenderer, mob_anim->sprite, &mob_anim->animRects[static_cast<int>(cFrame)], &tmpMobPos);
				
			}

			if (pause == false) {
				mob_anim->current_frame = mob_anim->current_frame + mob_anim->delta;
			}

			tmpMob->setTarget(target);
			tmpMob->scanTarget(tmpMob->MOB_NAME);

			if (tmpMob->collider.findCollision(&target->playerRect, tmpMob->MOB_NAME, NULL, NULL) == true && tmpMob->state != attack && cFrame > 0.0f) {
				if (tmpMob->state != attack) {
					target->KnockBack();
				}
				tmpMob->state = attack;

			}
			else {
				tmpMob->state = idle;
			}

			if ((tmpMob->state == idle || tmpMob->state == walking) && cFrame > 0.0f) {
				tmpMob->roamAround();
			}

			if (cFrame > 0) {
				tmpMob->Life.LifeTime = (SDL_GetTicks() / 1000) - tmpMob->Life.Birth;
			}

			if (cFrame > 0.0f) {
				tmpMob->Life.eop += 2.5;
			}

			if (tmpMob->Life.eop >= 255) {
				tmpMob->Life.eop = 255;
			}
		}

		
	}

	void loadMobList() {
		tinyxml2::XMLDocument doc;
		tinyxml2::XMLElement* pRoot;
		doc.LoadFile("data\\mobs\\mobs.zenx");

		pRoot = doc.FirstChildElement("mobs")->FirstChildElement("mob");

		for (; pRoot != nullptr; pRoot = pRoot->NextSiblingElement("mob")) {
			std::string mob_name = pRoot->Attribute("name");
			int mob_id = pRoot->IntAttribute("id");
			int mob_init_health = pRoot->IntAttribute("Health");
			int mob_init_armour = pRoot->IntAttribute("Armour");
			int mob_init_mana = pRoot->IntAttribute("Mana");

			MOBS[mob_name].MOB_NAME = mob_name;
			MOBS[mob_name].MOB_ID = mob_id;
			MOBS[mob_name].Life.Health = mob_init_health;
			MOBS[mob_name].Life.Armour = mob_init_armour;
			MOBS[mob_name].Life.Mana = mob_init_mana;

			MOBS_LIST[MOBS_LIST.size()] = mob_name;

			tinyxml2::XMLElement* aRoot = pRoot->FirstChildElement("animations")->FirstChildElement("anim");

			for (; aRoot != nullptr; aRoot = aRoot->NextSiblingElement("anim")) {
				std::string sprite_anim_name = aRoot->Attribute("name");
				std::string sprite_filepath = aRoot->Attribute("sprite");
				float sprite_delta = aRoot->FloatAttribute("delta");
				float sprite_max_frames = aRoot->FloatAttribute("max_frames");
				int sprite_width = aRoot->IntAttribute("sprite_width");
				int sprite_height = aRoot->IntAttribute("sprite_height");

				MOBS[mob_name].anims[sprite_anim_name.c_str()].display_surface = NULL;
				MOBS[mob_name].anims[sprite_anim_name.c_str()].loadTexture(sprite_filepath.c_str(), gRenderer);
				MOBS[mob_name].anims[sprite_anim_name.c_str()].display_surface = IMG_Load(sprite_filepath.c_str());
				MOBS[mob_name].anims[sprite_anim_name.c_str()].max_frames = sprite_max_frames;
				MOBS[mob_name].anims[sprite_anim_name.c_str()].delta = sprite_delta;
				MOBS[mob_name].anims[sprite_anim_name.c_str()].yfactorup = aRoot->IntAttribute("yfactorup");

				if (MOBS[mob_name].anims[sprite_anim_name.c_str()].sprite == NULL) {
					printf("SDL Error: %s", SDL_GetError());
				}
				else {
					int tmp_f = static_cast<int>(sprite_max_frames);
					MOBS[mob_name].addAnimation(&MOBS[mob_name].anims[sprite_anim_name.c_str()], 0, tmp_f, sprite_width, sprite_height);
					MOBS[mob_name].playerRect.y = 220;
					MOBS[mob_name].playerRect.x = 414;
					MOBS[mob_name].playerRect.w = sprite_width;
					MOBS[mob_name].playerRect.h = sprite_height;
				}

				try {
					std::string tmpAnimName = pRoot->FirstChildElement("animations")->FirstChildElement("default_state")->Attribute("name");
					MOBS[mob_name].current_animation = &MOBS[mob_name].anims[tmpAnimName];
				}
				catch (...) {
					printf("Error setting default animation! Assuming idle_left has been added!\n");
				}
			}
			tinyxml2::XMLElement* anims = pRoot->FirstChildElement("anim");
		}
	}
	GAME(SDL_Renderer* renderer = NULL) {
		if (renderer != NULL) {
			gRenderer = renderer;
		}

		spawnmgr.MOBS = &MOBS;
		spawnmgr.MOBS_LIST = &MOBS_LIST;

		SDL_CreateThread(spawn_manage, "LazyThread", NULL);
	}
};