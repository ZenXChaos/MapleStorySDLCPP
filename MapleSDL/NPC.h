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
#include "ItemDrop.hpp"
#include "HUD.hpp"
#include "GameObject.h"
#include "Entity.hpp"
#include "Camera.hpp"
#include "Skill.h"
#include "Entity.hpp"


class NPC : public Entity
{
public:
	bool interactable = true;

	void Draw(bool oc = false);

	std::map<std::string, AnimatedSprite> animations;

	void Core() override;

	NPC();
	virtual ~NPC();
};

