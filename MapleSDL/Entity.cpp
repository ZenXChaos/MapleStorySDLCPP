#include <SDL.h>
#include <SDL_image.h>
#include <stdio.h>
#include <stdlib.h>
#include <string>
#include <map>
#include <vector>

using namespace std;

#include "GameUtils.h"
#include "RelativeSpace.h"
#include "AnimatedSprite.h"
#include "Entity.h"

#define DEBUG_MOBTRANSIT_RAYCAST 0

void Entity::Draw() {
	switch (this->State) {
	case Idle:
		this->animations.at("idle").Animate(pos, 0, NULL, this->FaceDirection, currFrameData);
		break;

	case Walking:
		this->animations.at("walk").Animate(pos, 0, NULL, this->FaceDirection, currFrameData);
		break;

	case Attacking:
		this->animations.at("attack").Animate(pos, 0, NULL, this->FaceDirection, currFrameData);
		break;
	}
}

void Entity::SetPositionY(int y) {
	this->pos.y = y;
}

void Entity::SetPositionX(int x) {
	this->pos.x = x;
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
					SDL_SetRenderDrawColor(this->animations.at("idle").getRenderer(), 0xFF, 0x00, 0x00, 0xFF);
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
					SDL_SetRenderDrawColor(this->animations.at("idle").getRenderer(), 0xFF, 0x00, 0x00, 0xFF);
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
	if (!chasing) {
		if (State == Idle || roaming) {
			Roam();
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