#include <string>
#include <map>
#include <vector>

using namespace std;

#include "MobExt.hpp"

#include <SDL.h>
#include <SDL_image.h>
#include <stdio.h>
#include <stdlib.h>
#include <string>
#include <map>
#include <vector>
#include "GameDebug.hpp"
#include "Global.h"

using namespace std;

#include "Input.hpp"
#include "MessageDispatch.hpp"
#include "GameUtils.hpp"
#include "RelativeSpace.hpp"
#include "AnimatedSprite.hpp"
#include "MISC\ItemDrop.hpp"
#include "GameObject.h"
#include "Entity.hpp"


template<class T>
MOB_EXT::MobExt<T>* MOB_EXT::MobExt<T>::push(T e)
{
	this->entities.insert(this->entities.end(), e);
}
