#include <stdio.h>
#include <stdlib.h>
#include <string>
#include <map>
#include <vector>
#include "LUtil.h"
#include <IL/il.h>
#include <IL/ilu.h>
#include "LTexture.h"

#include "RelativeSpace.hpp"
#include "GameUtils.hpp"
#include "MessageDispatch.hpp"
#include "ItemDrop.hpp"
#include "Input.hpp"
#include "Entity.hpp"

#include "HelperFunctions.hpp"

void MessageDispatch::RegisterMessage(std::string msg, void(*callback)(Entity *e), Entity *context) {
	DispatchedMessage dm;
	dm.callback = callback;
	callback(context);
	this->messages.push_back(dm);
}