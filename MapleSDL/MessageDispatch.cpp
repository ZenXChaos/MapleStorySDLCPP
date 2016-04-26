#include <SDL.h>
#include <SDL_image.h>
#include <stdio.h>
#include <stdlib.h>
#include <string>
#include <map>
#include <vector>
#include <tinyxml2.h>
#include <Box2D/Box2D.h>

#pragma comment(lib, "tinyxml2.lib")

using namespace std;

#include "MessageDispatch.h"
#include "GameUtils.h"
#include "RelativeSpace.h"
#include "AnimatedSprite.h"
#include "Entity.h"
#include "SpawnManager.h"
#include "Box.h"
#include "Game.h"

Uint32 MD_Time::GetAge(Uint32 tick) {
	return (tick - birth) / 1000;
}



void MessageDispatch::RegisterMessage(std::string msg, void(*callback)(Entity *e), Entity *context) {
	DispatchedMessage dm;
	dm.callback = callback;
	this->messages.push_back(dm);
}