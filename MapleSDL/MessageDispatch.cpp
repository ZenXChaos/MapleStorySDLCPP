#include <SDL.h>
#include <SDL_image.h>
#include <stdio.h>
#include <stdlib.h>
#include <string>
#include <map>
#include <vector>
#include <tinyxml2.h>
#include <Box2D/Box2D.h>
#include "GameDebug.hpp"
#include "Global.h"

#pragma comment(lib, "tinyxml2.lib")

using namespace std;

#include "Input.hpp"
#include "MessageDispatch.hpp"
#include "GameUtils.hpp"
#include "RelativeSpace.hpp"
#include "AnimatedSprite.hpp"
#include "MISC/ItemDrop.hpp"
#include "GameObject.h"
#include "Entity.hpp"
#include "SpawnManager.hpp"
#include "Box.hpp"
#include "Game.hpp"


void MessageDispatch::RegisterMessage(std::string msg, void(*callback)(Entity *e), Entity *context, Uint32 timestamp) {
	DispatchedMessage dm;
	dm.callback = callback;
	callback(context);
	this->messages.push_back(dm);

#ifdef DEBUG_ENTITYDISPATCHMSG_R
	printf("Mob (%s) received Dispatch Message: %s!\n", context->uniq_id.c_str(), msg.c_str());
#endif
}