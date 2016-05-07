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



void Skill::Draw(SDL_Rect pos)
{
	pos.x += this->bindedEffectPos->x;
	pos.y += this->bindedEffectPos->y;
	//pos.w += this->bindedEffectPos->w;
	//pos.h += this->bindedEffectPos->h;

	this->sprite.Animate(pos, 0, NULL, SDL_FLIP_NONE, nullptr);
	if (this->sprite.isFinishedPlaying() == true) {
		//delete this;
		this->active = false;
	}
}

void Skill::OnCreate() {
	this->target->ApplyDamage(this->healthEffector*-1);
}

void Skill::Core() {
	if (!this->active) {
		return;
	}

	this->Draw({ 0 });
}

Skill::Skill() : GAMEObject()
{
}

Skill::~Skill()
{
	SDL_DestroyTexture(this->sprite.texture);
}