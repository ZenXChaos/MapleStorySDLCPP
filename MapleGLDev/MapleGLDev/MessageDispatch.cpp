#include <stdio.h>
#include <stdlib.h>
#include <string>
#include <map>
#include <vector>
#include "LUtil.h"
#include <IL/il.h>
#include <IL/ilu.h>
#include "LTexture.h"

#include "RelativeSpace.h"
#include "GameUtils.h"
#include "MessageDispatch.h"
#include "ItemDrop.hpp"
#include "Entity.h"

#include "HelperFunctions.h"

void MessageDispatch::RegisterMessage(std::string msg, void(*callback)(Entity *e), Entity *context) {
	DispatchedMessage dm;
	dm.callback = callback;
	callback(context);
	this->messages.push_back(dm);
}