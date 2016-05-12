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
#include "GameObject.h"
#include "ItemDrop.hpp"
#include "HUD.hpp"
#include "GameObject.h"
#include "Entity.hpp"
#include "Camera.hpp"
#include "Skill.h"



void Skill::Draw(SDL_Rect pos)
{
	// Always add bindedPos position offset to skill local position.
	pos.x += this->bindedEffectPos.x;
	pos.y += this->bindedEffectPos.y;

	// Animate the Skill's sprite
	this->sprite.Animate(pos, 0, NULL, SDL_FLIP_NONE, nullptr);

	// Set GAMEObject::active = false if animation finished playing
	if (this->sprite.isFinishedPlaying() == true) {
		this->active = false;
	}
}

void Skill::OnCreate() {
	// Apply damage to the target
	this->target->ApplyDamage(this->healthEffector*-1);
}

void Skill::Core() {
	// Main Skill Handle
	// Return and do nothing if GAMEObject not active.
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