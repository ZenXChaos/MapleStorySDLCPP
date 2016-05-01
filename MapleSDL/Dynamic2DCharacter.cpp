#include <SDL.h>
#include <SDL_image.h>
#include <stdio.h>
#include <stdlib.h>
#include <string>
#include <map>
#include <vector>
#include <Box2D/Box2D.h>
#include "GameDebug.h"
#include "Global.h"

using namespace std;


#include "Input.h"
#include "MessageDispatch.h"
#include "Box.h"
#include "GameUtils.h"
#include "RelativeSpace.h"
#include "AnimatedSprite.h"
#include "MISC/ItemDrop.hpp"
#include "Entity.hpp"
#include "SpawnManager.h"
#include "GameMap.h"
#include "HUD.h"
#include "Game.h"
#include "Dynamic2DCharacter.h"
#include "Global.h"



void Dynamic2DCharacter::DrawParts(SDL_Rect worldPos)
{
	SDL_Rect localPosHead = worldPos;
	localPosHead.x += this->d2_BodyParts["head"].relX;
	localPosHead.y += this->d2_BodyParts["head"].relY;

	SDL_Rect localPosBody = worldPos;
	localPosBody.x += this->d2_BodyParts["body"].relX;
	localPosBody.y += this->d2_BodyParts["body"].relY;

	SDL_Rect localPosArms = worldPos;
	localPosArms.x += this->d2_BodyParts["arms"].relX;
	localPosArms.y += this->d2_BodyParts["arms"].relY;

	this->d2_BodyParts["body"].bp_Animations["idle"].Animate(localPosBody, 0, NULL, SDL_FLIP_NONE, NULL);
	this->d2_BodyParts["head"].bp_Animations["idle"].Animate(localPosHead, 0, NULL, SDL_FLIP_NONE, NULL);
	this->d2_BodyParts["arms"].bp_Animations["idle"].Animate(localPosArms, 0, NULL, SDL_FLIP_NONE, NULL);
}

Dynamic2DCharacter::Dynamic2DCharacter(Player* mainPlayer) : Player(*mainPlayer)
{
	Dynamic2DBodyPart head;
	head.bp_Animations["idle"] = *(new AnimatedSprite());
	head.bp_Animations["idle"].BuildAnimation(0, 3, 123 / 3, 37, 0.1f);

	Dynamic2DBodyPart body;
	body.bp_Animations["idle"] = *(new AnimatedSprite());
	body.bp_Animations["idle"].BuildAnimation(0, 3, 66 / 3, 31, 0.1f);
	body.relX = 7;
	body.relY = 35;

	Dynamic2DBodyPart arms;
	arms.bp_Animations["idle"] = *(new AnimatedSprite());
	arms.bp_Animations["idle"].BuildAnimation(0, 3, 31 / 3, 19, 0.1f);
	arms.relX = 23;
	arms.relY = 37;

	this->d2_BodyParts["head"] = head;
	this->d2_BodyParts["body"] = body;
	this->d2_BodyParts["arms"] = arms;
}


Dynamic2DCharacter::~Dynamic2DCharacter()
{
}
