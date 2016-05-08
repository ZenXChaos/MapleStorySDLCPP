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
#include "HUD.hpp"
#include "GameObject.h"
#include "Entity.hpp"
#include "Camera.hpp"
#include "Skill.h"
#include "Game.hpp"
#include "NPC.h"



NPC::NPC()
{
	this->animations["default_state"].LoadTexture("content/npc/01/stand_0.png", m_gRenderer);
	this->animations["default_state"].BuildAnimation(0, 1, 56, 70, 0.0f);
}

void NPC::Core() {
	this->animations["default_state"].Animate(this->pos, 0, NULL, SDL_RendererFlip::SDL_FLIP_NONE, nullptr);
}

NPC::~NPC()
{
}
