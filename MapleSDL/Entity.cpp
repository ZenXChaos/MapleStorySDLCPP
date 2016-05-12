#include <SDL.h>
#include <SDL_image.h>
#include <stdio.h>
#include <stdlib.h>
#include <string>
#include <map>
#include <vector>
#include <algorithm>
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

void Entity::Draw(bool oc) {

	if (oc) {
		currentAnimation = &this->animations.at("idle");
		return;
	}

	if (this->alive == false) {
		return;
	}

	// Play animation according to current state.
	switch (this->State) {
	case Idle:
	case Chasing:
		// Chasing state is a temporary transition state. Plays Idle animation
		this->animations.at("idle").Animate(pos, 0, NULL, this->FaceDirection, currFrameData);
		currentAnimation = &this->animations.at("idle");
		break;

	case Walking:
		this->animations.at("walk").Animate(pos, 0, NULL, this->FaceDirection, currFrameData);
		currentAnimation = &this->animations.at("walk");
		break;

	case Attacking:
		this->animations.at("attack").Animate(pos, 0, NULL, this->FaceDirection, currFrameData);
		currentAnimation = &this->animations.at("attack");
		break;

	case Recovery:
		this->animations.at("hit").Animate(pos, 0, NULL, this->FaceDirection, currFrameData);
		currentAnimation = &this->animations.at("hit");
		this->DrawHealth();
		break;

	case Death:
		this->animations.at("die").Animate(pos, 0, NULL, this->FaceDirection, currFrameData);
		currentAnimation = &this->animations.at("die");
		break;
	}
	
	
}

void Entity::SetPositionY(int y) {
	// Manually set position Y
	this->pos.y = y;
}

void Entity::SetPositionX(int x) {
	// Manually set position X
	this->pos.x = x;
}

SDL_Rect Entity::GetPosition() {
	// Return current XY position
	return this->pos;
}

int Entity::GetPositionX() {
	// Return current Y position
	return this->pos.x;
}

int Entity::GetPositionY() {
	// Return current Y position
	return this->pos.y;
}


int Entity::GetWidth() {
	// Return current animation width
	return this->currFrameData->w;
}

int Entity::GetHeight() {
	// Return current animation height
	return this->currFrameData->h;
}

void Entity::Station() {

	//If entity is walking, force it to idle.
	if (this->State == Walking) {
		this->State = Idle;
	}
}

void Entity::Roam() {
	if (roaming == true) {
		if (nextTransitLocation.x != pos.x) {
			this->WalkTowards(nextTransitLocation);
#ifdef DEBUG_MOBTRANSIT_ROAMNOTREACHED
			printf("Mob (%s) transiting to location {%i,%i}\n", this->uniq_id.c_str(), nextTransitLocation.x, nextTransitLocation.y);
#endif

#ifdef DEBUG_MOBTRANSIT_RAYCAST
			//If debug option set, show squares leading up to the target location entity is roaming to.
			SDL_Rect fillRect = nextTransitLocation;
			fillRect.y = this->pos.y;
			fillRect.w = this->currFrameData->w;
			fillRect.h = this->currFrameData->h;

			if (this->pos.x < nextTransitLocation.x) {
				for (int i = this->pos.x + ((this->currFrameData->w / 2) + (this->currFrameData->w / 4)); i < nextTransitLocation.x-5; i += 12) {
					SDL_Rect tmpPos;
					tmpPos.y = this->pos.y + (this->currFrameData->h / 2)-5;
					tmpPos.x = i;
					tmpPos.w = 10;
					tmpPos.h = 10;
					SDL_SetRenderDrawColor(this->animations.at("idle").getRenderer(), nextTransitLocation.x, nextTransitLocation.x*2, nextTransitLocation.x, 0xFF);
					SDL_RenderDrawRect(this->animations.at("idle").getRenderer(), &tmpPos);
					SDL_RenderFillRect(this->animations.at("idle").getRenderer(), &tmpPos);
					SDL_RenderDrawRect(this->animations.at("idle").getRenderer(), &fillRect);
					SDL_SetRenderDrawColor(this->animations.at("idle").getRenderer(), 0xFF, 0xFF, 0xFF, 0xFF);
				}
			}
			else {
				for (int i = this->pos.x + (this->currFrameData->w / 4); i > nextTransitLocation.x+this->currFrameData->w+5; i -= 12) {
					SDL_Rect tmpPos;
					tmpPos.y = this->pos.y + (this->currFrameData->h / 2) - 5;
					tmpPos.x = i;
					tmpPos.w = 10;
					tmpPos.h = 10;
					SDL_SetRenderDrawColor(this->animations.at("idle").getRenderer(), nextTransitLocation.x, nextTransitLocation.x*2, nextTransitLocation.x, 0xFF);
					SDL_RenderDrawRect(this->animations.at("idle").getRenderer(), &tmpPos);
					SDL_RenderFillRect(this->animations.at("idle").getRenderer(), &tmpPos);
					SDL_RenderDrawRect(this->animations.at("idle").getRenderer(), &fillRect);
					SDL_SetRenderDrawColor(this->animations.at("idle").getRenderer(), 0xFF, 0xFF, 0xFF, 0xFF);
				}
			}
#endif

		}
		else {
			this->roaming = false;
			this->State = Idle;
			this->roamDelayIndex = this->age();
#ifdef DEBUG_MOBTRANSIT_ROAMREACHED
			printf("Mob (%s) reached location {%i,%i}\n", this->uniq_id.c_str(), nextTransitLocation.x, nextTransitLocation.y);
#endif
		}
	}
	else {

		// If the last time the mob roamed is greater than or equal to the delay
		if(this->age() - this->roamDelayIndex >= this->roamDelay ){

			// Set new roam index
			this->roamDelayIndex = this->age();

			// Generate a random x location to move to
			nextTransitLocation.x = GameUtils::RandomIntegerRange(0, 900);

			// If the generated location is less than the minimum transit value, regenerate location
 			while (nextTransitLocation.x < this->pos.x + this->minRoamTransit && nextTransitLocation.x > pos.x) {
 
 				nextTransitLocation.x = GameUtils::RandomIntegerRange(0, 900);
 			}

			// If the generated location is greater than the maximum transit value, regenerate location
			while (nextTransitLocation.x > this->pos.x - this->minRoamTransit && nextTransitLocation.x < pos.x) {

				nextTransitLocation.x = GameUtils::RandomIntegerRange(0, 900);
			}
			roaming = true;
		}
	}
}

void Entity::WalkTowards(SDL_Rect topos) {

	// Walk towards topos X
	if (this->State == Attacking || topos.x < 0) {
		return;
	}
	else {
		State = Walking;
	}

	if (topos.x < pos.x) {
		this->Walk(Left);
	}
	else
	{
		this->Walk(Right);
	}

}

void Entity::WalkAway(SDL_Rect frompos) {
	// Walk opposite direction of frompos X
	if (this->State == Attacking || frompos.x < 0) {
		return;
	}
	else {
		State = Walking;
	}

	if (frompos.x < pos.x) {
		this->Walk(Right);
	}
	else
	{
		this->Walk(Left);
	}

}

void Entity::Walk(FlipDirection direction) {

	// Walk

	Direction = direction;
	if (this->State == Attacking) {
		// Prevent entity from walking while attacking
		return;
	}
	else {
		State = Walking;
	}

	// If uses acceleration, gradully increase speed.
	if (this->usesAccel) {
		if (this->walkSpeed < this->maxWalkSpeed) {
			this->walkSpeed += this->walkSpeedAccel;
		}
	}
	else {
		this->walkSpeed = maxWalkSpeed;
	}

	// If facing right
	if (direction == Right) {
		this->pos.x += this->walkSpeed;
		this->FaceDirection = SDL_FLIP_HORIZONTAL;
	}

	// If facing left
	else {
		this->pos.x -= this->walkSpeed;
		this->FaceDirection = SDL_FLIP_NONE;
	}


}

void Player::IdentifyMobs() {

	// Identify which mobs are in range
	this->closestMob = nullptr;
	this->inRange.clear();
	
	int dist = 10000;
	for (size_t i = 0; i < this->spawned->size(); i++) {
		Entity* mob = &this->spawned->at(i);
		if (mob == nullptr) {
			break;
		}

		// If Entity is on the right of player
		if (mob->GetPositionX() > this->pos.x) {
			// If the entity is in range from the right and is the closest mob, set closestMob
			if (mob->GetPositionX() - this->pos.x <= this->attackRange && dist > mob->GetPositionX() - this->pos.x) {
				this->closestMob = mob;
				dist = mob->GetPositionX() - this->pos.x;
			}
			
			// If the entity is in range from the right, add to inRange
			if (mob->GetPositionX() - this->pos.x <= this->attackRange) {
				this->inRange.insert(this->inRange.end(), mob);
			}
		}
		// If Entity is on the left of player
		else{
			// If the entity is in range from the left and is the closest mob, set closestMob
			if (this->pos.x - mob->GetPositionX() <= this->attackRange && dist > this->pos.x - mob->GetPositionX()) {
				this->closestMob = mob;
				dist = this->pos.x - mob->GetPositionX();
			}

			// If the entity is in range from the left, add to inRange
			if (this->pos.x - mob->GetPositionX() <= this->attackRange) {
				this->inRange.insert(this->inRange.end(), mob);
			}
		}

	}
}

void Player::IdentifyItemDrops() {
	//for(size_t i = 0; i < )

	std::vector<GameItemDrop> itemsInRange;
	for (size_t i = 0; i < gameItems.objects.size(); i++) {
		GameItemDrop* drop = new GameItemDrop();
		if (i >= gameItems.objects.size()) {
			return;
		}
		else {
			if (playerInput->IsKeyPressed(SDL_SCANCODE_Z) && gameItems.objects.at(i)->active == true) {
				memcpy(drop, gameItems.objects.at(i), sizeof(gameItems.objects.at(i)));
				drop->sprite = gameItems.objects.at(i)->sprite;

				this->InventoryManager.Items.insert(this->InventoryManager.Items.end(), *drop);
				gameItems.objects.erase(gameItems.objects.begin()+i);
			}
			drop->active = true;
		}
	}
}

void Entity::TakeHit() {
	// Take damage
	this->State = EntityState::Recovery;
	this->recoveryIndex = 1.5f;
}

void Entity::PrepKill() {
	// Death transition state
	this->State = EntityState::Death;
}

void Entity::GenUniqID()
{
	// Generate a unique ID for the entity to be identified
	this->uniq_id = GameUtils::UniqID() + GameUtils::UniqID();
}

void Entity::DrawHealth()
{
	//Draw Entity Health
	HUD_FlowPanel expFlowPanel;
	expFlowPanel.width = 400;
	expFlowPanel.spacingX = -2;

	//Convert the health (from int) to string
	char *entityHealth_S = (char*)malloc(sizeof(char) * 80);
	_itoa_s(this->Life.Health, entityHealth_S, 80, 10);
	int sp = strlen(entityHealth_S);

	//Loop through all characters
	for (int i = 0; i < sp; i++) {

		//Assign a unique id to HUDObject based on character index
		char *tName = (char*)malloc(sizeof(char) * 80);
		_itoa_s(i, tName, 80, 10);

		char *lHit_S = (char*)malloc(sizeof(char) * 80);
		_itoa_s(this->lastHit, lHit_S, 80, 10);

		std::string objename = this->uniq_id + ".";
		objename += tName;
		objename += ".";
		objename += lHit_S;
		objename += ".";

		//Create a HUDObject
		//Assign DamageNo.x
		HUDObject ItemNo;
		std::string itemno = "DamageNo.";
		itemno += entityHealth_S[i];
		ItemNo.sprites = &HUDElements[itemno];
		objename += itemno;
		ItemNo.e_ID = objename;

		//Apply transitionUp animation
		HUD_Animation<HUDObject> hanim;
		hanim.RegisterHUDEffect(&ItemNo);
		hanim.ApplyEffect(&ItemNo, HUD_ANIM_DMGNO_TransitionUp);
		expFlowPanel.AddObject(ItemNo);
	}


	expFlowPanel.DrawPanel(this->pos.x, this->pos.y);
}

void Entity::Core()
{
	// Main Entity HANDLE

	// If current animation not set, return to prevent issues
	if (currentAnimation == nullptr) {
		return;
	}
	
	// Get the time in milliseconds since running (convert to float).
	tick = static_cast<float>(SDL_GetTicks());

	//If entity has become alert, chase player
	if (this->alert == true) {
		this->State = EntityState::Chasing;
	}

	// If not dead
	if (this->State != EntityState::Death) {
		switch (this->State) {
		case EntityState::Recovery:
			// Recovery : Shock from being hit
			if (this->recoveryIndex > 0) {
				this->recoveryIndex -= this->currentAnimation->getDelta();
			}
			else {

				// After recovery from a hit, set to transitional state, EntityState::Chasing
				// Chase for 120 seconds
				this->chasing = true;
				this->State = EntityState::Chasing;
				this->recoveryIndex = 120.0f;
			}

			break;

		// If dead
		case EntityState::Death:

			// If the death animation is at least 90% complete, kill the entity
			if (this->currentAnimation->percentComplete() >= 90) {
				this->Kill();
			}
			break;
		}


		if (this->State == EntityState::Recovery) {
			// Do nothing
		}

		// If entity is chasing
		else if (this->chasing || this->State == EntityState::Chasing) {

			// If chase time has not run up, count new time
			if (this->recoveryIndex > 0) {
				this->recoveryIndex -= this->currentAnimation->getDelta();

				// If on the left of player, WalkTowards player until 1 pixel away
				if (this->pos.x > GLOBAL_MMORPG_GAME::m_Player->pos.x) {
					if (this->pos.x - 1 != GLOBAL_MMORPG_GAME::m_Player->pos.x) {
						this->WalkTowards(GLOBAL_MMORPG_GAME::m_Player->pos);
					}
					else {
						this->Station();
					}
				}

				// If on the right of player, WalkTowards player until 1 pixel away
				else {

					if (this->pos.x + 1 != GLOBAL_MMORPG_GAME::m_Player->pos.x) {
						this->WalkTowards(GLOBAL_MMORPG_GAME::m_Player->pos);
					}
					else {
						this->Station();
					}
				}


			}
			// If chase time has run up, set chasing = false, State = EntityState::Idle
			else {
				this->chasing = false;
				this->State = EntityState::Idle;
			}
		}

		else {

			// If not chasing a player, roam around
			if (!chasing) {

				// Only if in idle or already roaming
				// Prevent entity from roaming while in the middle of an attack or some custom state
				if (State == Idle || roaming) {
					Roam();
				}
			}
		}
	}
	else {

		// If dead, and animation 95% complete, set alive to false. Entity will be destroyed next frame.
		if (this->currentAnimation->percentComplete() >= 95 || this->currentAnimation->isFinishedPlaying() == true && this->alive == true) {
			this->alive = false;
		}
	}
}

void Entity::Kill() {
	this->alive = false;
}

void Player::AttackMob() {
	// Atack the closest mob
	Entity* tmpE = this->closestMob;
	if (tmpE == nullptr) {
		return;
	}
	tmpE->dispatch_message.RegisterMessage("IsHit", &IsHit, this->closestMob);
	this->attacking = true;

	// Use a skill : mageclaw
	Skill *sk = new Skill();
	(*sk).sprite = HUDElements["mage.skill.mageclaw"];
	(*sk).BindEntity(tmpE);
	skillGameObjects.Instantiate(sk);

	printf("Mob hit with claw: %s\n", tmpE->uniq_id.c_str());
	this->closestMob = nullptr;
}


void Player::Core()
{
	// MAIN PLAYER HANDLE

	IdentifyItemDrops();
	// If player is attacking.
	if (this->State == EntityState::Attacking) {

		// If the attack animation is finished, set state to EntityState::Idle and attacking = false
		if (this->currentAnimation->isFinishedPlaying()) {
			this->State = EntityState::Idle;
			this->attacking = false;
		}
		else {

			// After animation is at least 60% complete, attack the mob.
			if (attacking == false) {
				if (this->closestMob != nullptr) {
					if ((this->FaceDirection == SDL_FLIP_NONE && this->closestMob->GetPositionX() < this->pos.x) || (this->FaceDirection == SDL_FLIP_HORIZONTAL && this->closestMob->GetPositionX() > this->pos.x + this->pos.w / 3)) {
						this->AttackMob();
						this->closestMob = nullptr;
					}
				}
			}
		}
	}

	// If attack key pressed, set to attack mode
	if (playerInput->IsKeyPressed(SDL_SCANCODE_C) && this->State != EntityState::Attacking) {
		float age = this->age();

		// If last attack time greater than delay time
		if (this->age() - this->lastAttack >= this->attackRecovery) {

			this->State = EntityState::Attacking;

			// Set last attack index
			this->lastAttack = this->age();
		}
		else {

#ifdef DEBUG_DENIED_PLAYERACTION
			printf("Player count not attack! Last attack `%F` < `%F\n", this->age() - this->lastAttack, this->attackRecovery);
#endif

		}
	}


	// Track time
	this->tick = static_cast<float>(SDL_GetTicks());
}
