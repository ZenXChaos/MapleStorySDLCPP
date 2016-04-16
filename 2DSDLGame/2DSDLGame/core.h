#pragma once

using namespace std;

#include <vector>
#include <map>

#include "config.h"
#include "sprite_framework.h"
#include "color.h"
#include "character_framework.h"
#include "mob_framework.h"

void limit_framerate(Uint32 tick) {
	if ((1000 / fps) > SDL_GetTicks() - tick) {
		SDL_Delay(1000 / fps - (SDL_GetTicks() - tick));
	}
}

