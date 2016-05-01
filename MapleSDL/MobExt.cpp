#include <string>
#include <map>
#include <vector>

using namespace std;

#include "MobExt.h"

#include <SDL.h>
#include <SDL_image.h>
#include <stdio.h>
#include <stdlib.h>
#include <string>
#include <map>
#include <vector>
#include "GameDebug.h"
#include "Global.h"

using namespace std;

#include "Input.h"
#include "MessageDispatch.h"
#include "GameUtils.h"
#include "RelativeSpace.h"
#include "AnimatedSprite.h"
#include "MISC\ItemDrop.hpp"
#include "Entity.hpp"


	template<class T>
	MOB_EXT::MobExt<T>* MOB_EXT::MobExt<T>::push(T e)
	{
		this->entities.insert(this->entities.end(), e);
	}
