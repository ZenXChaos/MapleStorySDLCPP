#include <SDL.h>
#include <SDL_image.h>
#include <stdio.h>
#include <stdlib.h>
#include <string>
#include <map>
#include <vector>

using namespace std;

#include "MessageDispatch.h"

Uint32 MD_Time::GetAge(Uint32 tick) {
	return (tick - birth) / 1000;
}