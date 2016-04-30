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
#include "MISC/ItemDrop.hpp"
#include "Entity.h"

void Entity::Draw() {
	if (this->alive == false) {
		return;
	}
	switch (this->State) {
	case Idle:
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
		break;

	case Death:
		this->animations.at("die").Animate(pos, 0, NULL, this->FaceDirection, currFrameData);
		currentAnimation = &this->animations.at("die");
		break;
	}
}

void Entity::SetPositionY(int y) {
	this->pos.y = y;
}

void Entity::SetPositionX(int x) {
	this->pos.x = x;
}

SDL_Rect Entity::GetPosition() {
	return this->pos;
}

int Entity::GetPositionY() {
	return this->pos.y;
}

int Entity::GetPositionX() {
	return this->pos.x;
}

int Entity::GetWidth() {
	return this->currFrameData->w;
}

int Entity::GetHeight() {
	return this->currFrameData->h;
}

void Entity::Station() {
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
			SDL_Rect fillRect = nextTransitLocation;
			fillRect.y = this->pos.y;
			fillRect.w = this->currFrameData->w;
			fillRect.h = this->currFrameData->h;
			/*SDL_SetRenderDrawColor(this->animations.at("idle").getRenderer(), 0xFF, 0xF2, 0x00, 0xFF);
			//SDL_RenderFillRect(gRenderer, &fillRect);
			SDL_RenderDrawRect(this->animations.at("idle").getRenderer(), &fillRect);
			SDL_SetRenderDrawColor(this->animations.at("idle").getRenderer(), 0xFF, 0xFF, 0xFF, 0xFF);
			*/
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
		if(this->age() - this->roamDelayIndex >= this->roamDelay ){
			this->roamDelayIndex = this->age();
			nextTransitLocation.x = GameUtils::RandomIntegerRange(0, 900);
 			while (nextTransitLocation.x < this->pos.x + this->minRoamTransit && nextTransitLocation.x > pos.x) {
 
 				nextTransitLocation.x = GameUtils::RandomIntegerRange(0, 900);
 			}

			while (nextTransitLocation.x > this->pos.x - this->minRoamTransit && nextTransitLocation.x < pos.x) {

				nextTransitLocation.x = GameUtils::RandomIntegerRange(0, 900);
			}
			roaming = true;
		}
	}
}

void Entity::AI() {
	tick = SDL_GetTicks();
	if (this->State != EntityState::Death) {
		switch (this->State) {
		case EntityState::Recovery:
			if (this->recoveryIndex > 0) {
				this->recoveryIndex -= currentAnimation->getDelta();
			}
			else {
				this->State = Idle;
			}

			break;

		case EntityState::Death:
			if (this->currentAnimation->percentComplete() >= 90) {
				this->Kill();
			}
			break;
		}
		if (this->State == EntityState::Recovery) {

		}
		else {
			if (!chasing) {
				if (State == Idle || roaming) {
					Roam();
				}
			}
		}
	}
	else {
		if (this->currentAnimation->percentComplete() >= 95 || this->currentAnimation->isFinishedPlaying() == true&& this->alive == true) {
			this->alive = false;
		}
	}
	
}

void Entity::WalkTowards(SDL_Rect topos) {

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
	Direction = direction;
	if (this->State == Attacking) {
		return;
	}
	else {
		State = Walking;
	}

	if (this->usesAccel) {
		if (this->walkSpeed < this->maxWalkSpeed) {
			this->walkSpeed += this->walkSpeedAccel;
		}
	}
	else {
		this->walkSpeed = maxWalkSpeed;
	}

	if (direction == Right) {
		this->pos.x += this->walkSpeed;
		this->FaceDirection = SDL_FLIP_HORIZONTAL;
	}
	else {
		this->pos.x -= this->walkSpeed;
		this->FaceDirection = SDL_FLIP_NONE;
	}


}

void Player::IdentifyMobs() {
	this->closestMob = nullptr;
	this->inRange.clear();
	
	int dist = -1;
	size_t i = 0;
	for (std::vector<Entity>::iterator mob = spawned->begin();mob != spawned->end();mob++) {
		if (mob->GetPositionX() > this->pos.x) {
			int mobd = mob->GetPositionX() - this->pos.x;
			if (mob->GetPositionX() - this->pos.x <= this->attackRange) {
				this->closestMob = &this->spawned->at(i);
				dist = mob->GetPositionX() - this->pos.x;
			}
		}else{
			if (this->pos.x - mob->GetPositionX() <= this->attackRange) {
				this->closestMob = &this->spawned->at(i);
				dist = this->pos.x - mob->GetPositionX();
			}
		}

		i++;
	}
}

void Entity::TakeHit() {
	this->State = EntityState::Recovery;
	this->recoveryIndex = 3.0f;
}

void Entity::PrepKill() {
	this->State = EntityState::Death;
}

void Entity::GenUniqID()
{
	this->uniq_id = GameUtils::UniqID() + GameUtils::UniqID();
}

void Entity::Kill() {
	this->alive = false;
}

void Player::ManageState() {
	if (this->State == EntityState::Attacking) {
		if (this->currentAnimation->isFinishedPlaying()) {
			this->State = EntityState::Idle;
			this->attacking = false;
		}else{
			float pdone = this->currentAnimation->percentComplete();
			if (this->currentAnimation->percentComplete() >= 50.0f && attacking == false) {
				if (this->closestMob != nullptr) {
					this->closestMob->dispatch_message.RegisterMessage("IsHit", &IsHit, this->closestMob);
					this->attacking = true;
				}
			}
		}
	}

	
	if (playerInput->IsKeyPressed(SDL_SCANCODE_C) && this->State != EntityState::Attacking) {
		Uint32 age = this->age();
		if (this->age() - this->lastAttack >= this->attackRecovery) {

			this->State = EntityState::Attacking;
			this->lastAttack = this->age();
		}else{

#ifdef DEBUG_DENIED_PLAYERACTION
			printf("Player count not attack! Last attack `%i` < `%i\n", static_cast<int>(this->age() - this->lastAttack), static_cast<int>(this->attackRecovery));
#endif
		
		}
	}

	this->tick = SDL_GetTicks();
}