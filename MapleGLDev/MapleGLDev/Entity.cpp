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

using namespace std;

#define DEBUG_MOBTRANSIT_RAYCAST 0

void Entity::Draw() {
	if (this->alive == false) {
		return;
	}
	switch (this->State) {
	case Idle:
		//this->animations->at("idle").Animate(pos, 0, NULL, this->FaceDirection, currFrameData);
		this->animations.at("idle").Animate(pos->x, pos->y, this->Direction);
		currentAnimation = &this->animations.at("idle");
		break;

	case Walking:
		//this->animations->at("walk").Animate(pos, 0, NULL, this->FaceDirection, currFrameData);
		this->animations.at("walk").Animate(pos->x, pos->y, this->Direction);
		currentAnimation = &this->animations.at("walk");
		break;

	case Attacking:
		//this->animations->at("attack").Animate(pos, 0, NULL, this->FaceDirection, currFrameData);
		this->animations.at("attack").Animate(pos->x, pos->y, this->Direction);
		currentAnimation = &this->animations.at("attack");
		break;

	case Recovery:
		this->animations.at("hit").Animate(pos->x, pos->y, this->Direction);
		currentAnimation = &this->animations.at("hit");
		break;

	case Death:
		this->animations.at("die").Animate(pos->x, pos->y, this->Direction);
		currentAnimation = &this->animations.at("die");
		break;
	}
}

void Entity::SetPositionY(GLfloat y) {
	this->pos->y = y;
}

void Entity::SetPositionX(GLfloat x) {
	this->pos->x = x;
}

LFRect Entity::GetPosition() {
	return *this->pos;
}

GLfloat Entity::GetPositionY() {
	return this->pos->y;
}

GLfloat Entity::GetPositionX() {
	return this->pos->x;
}

GLfloat Entity::GetWidth() {
	return this->currFrameData->w;
}

GLfloat Entity::GetHeight() {
	return this->currFrameData->h;
}

void Entity::Station() {
	if (this->State == Walking) {
		this->State = EntityState::Idle;
	}
}
void Entity::Tick() {
	this->tick++;
}
void Entity::Roam() {
	if (roaming == true) {
		if (nextTransitLocation.x != pos->x) {
			this->WalkTowards(nextTransitLocation);

#ifdef DEBUG_MOBTRANSIT_RAYCAST
			LFRect fillRect = nextTransitLocation;
			fillRect.y = this->pos->y;
			fillRect.w = this->currFrameData->w;
			fillRect.h = this->currFrameData->h;
			/*SDL_SetRenderDrawColor(this->animations->at("idle").getRenderer(), 0xFF, 0xF2, 0x00, 0xFF);
			//SDL_RenderFillRect(gRenderer, &fillRect);
			SDL_RenderDrawRect(this->animations->at("idle").getRenderer(), &fillRect);
			SDL_SetRenderDrawColor(this->animations->at("idle").getRenderer(), 0xFF, 0xFF, 0xFF, 0xFF);
			*/
			if (this->pos->x < nextTransitLocation.x) {
				for (GLfloat i = this->pos->x + ((this->currFrameData->w / 2) + (this->currFrameData->w / 4)); i < nextTransitLocation.x - 5; i += 0.4f) {
					LFRect tmpPos;
					tmpPos.y = this->pos->y + (this->currFrameData->h / 2) - 5;
					tmpPos.x = i;
					tmpPos.w = 10;
					tmpPos.h = 10;
					/*SDL_SetRenderDrawColor(this->animations->at("idle").getRenderer(), 0xFF, 0x00, 0x00, 0xFF);
					SDL_RenderDrawRect(this->animations->at("idle").getRenderer(), &tmpPos);
					SDL_RenderFillRect(this->animations->at("idle").getRenderer(), &tmpPos);
					SDL_RenderDrawRect(this->animations->at("idle").getRenderer(), &fillRect);
					SDL_SetRenderDrawColor(this->animations->at("idle").getRenderer(), 0xFF, 0xFF, 0xFF, 0xFF
					*/
				}
			}
			else {
				for (GLfloat i = this->pos->x + (this->currFrameData->w / 4); i > nextTransitLocation.x + this->currFrameData->w + 5; i -= 0.4f) {
					LFRect tmpPos;
					tmpPos.y = this->pos->y + (this->currFrameData->h / 2) - 5;
					tmpPos.x = i;
					tmpPos.w = 10;
					tmpPos.h = 10;
					/*SDL_SetRenderDrawColor(this->animations->at("idle").getRenderer(), 0xFF, 0x00, 0x00, 0xFF);
					SDL_RenderDrawRect(this->animations->at("idle").getRenderer(), &tmpPos);
					SDL_RenderFillRect(this->animations->at("idle").getRenderer(), &tmpPos);
					SDL_RenderDrawRect(this->animations->at("idle").getRenderer(), &fillRect);
					SDL_SetRenderDrawColor(this->animations->at("idle").getRenderer(), 0xFF, 0xFF, 0xFF, 0xFF);*/
				}
			}
#endif
		}
		else {
			this->roaming = false;
			this->State = EntityState::Idle;
			this->roamDelayIndex = this->age();
		}
	}
	else {
		int age = this->age();
		if (age - this->roamDelayIndex >= this->roamDelay) {
			this->roamDelayIndex = this->age();
			nextTransitLocation.x = static_cast<GLfloat>(GameUtils::RandomIntegerRange(0, 900));
			while (nextTransitLocation.x < this->pos->x + static_cast<GLfloat>(this->minRoamTransit) && nextTransitLocation.x > pos->x) {

				nextTransitLocation.x = static_cast<GLfloat>(GameUtils::RandomIntegerRange(0, 900));
			}

			while (nextTransitLocation.x > this->pos->x - this->minRoamTransit && nextTransitLocation.x < pos->x) {

				nextTransitLocation.x = static_cast<GLfloat>(GameUtils::RandomIntegerRange(0, 900));
			}
			roaming = true;
		}
	}
}

void Entity::AI() {
	//tick = SDL_GetTicks();
	if (this->State != EntityState::Death) {
		switch (this->State) {
		case EntityState::Recovery:
			if (this->recoveryIndex > 0.0f) {
				recoveryIndex -= currentAnimation->getDelta();
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
		if (this->currentAnimation->percentComplete() >= 95 || this->currentAnimation->isFinishedPlaying() == true && this->alive == true) {
			this->alive = false;
		}
	}

	this->Tick();
}

void Entity::WalkTowards(LFRect topos) {

	if (this->State == EntityState::Attacking || topos.x < 0) {
		return;
	}
	else {
		State = EntityState::Walking;
	}

	if (topos.x < pos->x) {
		this->Walk(FlipDirection::Left);
	}
	else
	{
		this->Walk(FlipDirection::Right);
	}

}

void Entity::WalkAway(LFRect frompos) {

	if (this->State == EntityState::Attacking || frompos.x < 0) {
		return;
	}
	else {
		State = EntityState::Walking;
	}

	if (frompos.x < pos->x) {
		this->Walk(FlipDirection::Right);
	}
	else
	{
		this->Walk(FlipDirection::Left);
	}

}

void Entity::Walk(FlipDirection direction) {
	if (this->State == EntityState::Attacking) {
		return;
	}
	else {
		State = EntityState::Walking;
	}

	if (this->usesAccel) {
		if (this->walkSpeed < this->maxWalkSpeed) {
			this->walkSpeed += this->walkSpeedAccel;
		}
	}
	else {
		this->walkSpeed = maxWalkSpeed;
	}

	if (direction == FlipDirection::Right) {
		this->pos->x += this->walkSpeed;
		//this->FaceDirection = SDL_FLIP_HORIZONTAL;
	}
	else {
		this->pos->x -= this->walkSpeed;
		//this->FaceDirection = SDL_FLIP_NONE;
	}

	Direction = direction;

}


void Player::IdentifyMobs() {
	this->closestMob = nullptr;
	this->inRange.clear();

	GLfloat dist = -1;
	size_t i = 0;
	for (std::vector<Entity*>::iterator mob = spawned->begin(); mob != spawned->end(); mob++) {
		if (mob[0]->GetPositionX() > this->pos->x && this->Direction == FlipDirection::Right) {
			GLfloat mobd = mob[0]->GetPositionX() - this->pos->x;
			if (mob[0]->GetPositionX() - this->pos->x <= this->attackRange && this->Direction == FlipDirection::Right && mob[0]->GetPositionX() - this->pos->x >= this->attackRange_Closest) {
				this->closestMob = this->spawned->at(i);
				dist = mob[0]->GetPositionX() - this->pos->x;
			}
		}
		else {
			if (this->pos->x - mob[0]->GetPositionX() <= this->attackRange && this->Direction == FlipDirection::Left && this->pos->x - mob[0]->GetPositionX() >= this->attackRange_Closest) {
				this->closestMob = &this->spawned->at(i)[0];
				dist = this->pos->x - mob[0]->GetPositionX();
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

void Entity::Kill() {
	this->alive = false;
}

void Player::ManageState() {
	if (this->State == EntityState::Attacking) {
		if (this->currentAnimation->isFinishedPlaying()) {
			this->State = EntityState::Idle;
			this->attacking = false;
		}
		else {
			float pdone = this->currentAnimation->percentComplete();
			if (this->currentAnimation->percentComplete() >= 50.0f && attacking == false) {
				if (this->closestMob->pos->x < this->pos->x && this->Direction == FlipDirection::Left) {
					this->closestMob->dispatch_message.RegisterMessage("IsHit", &IsHit, this->closestMob);
					this->attacking = true;
				}
				else if (this->closestMob->pos->x > this->pos->x && this->Direction == FlipDirection::Right) {
					this->closestMob->dispatch_message.RegisterMessage("IsHit", &IsHit, this->closestMob);
					this->attacking = true;
				}
			}
		}
	}

	/*
	if (playerInput->IsKeyPressed(SDL_SCANCODE_C) && this->State != EntityState::Attacking) {
		this->State = EntityState::Attacking;
	}
	*/
}