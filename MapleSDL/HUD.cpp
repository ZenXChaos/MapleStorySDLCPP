#include <SDL.h>
#include <SDL_image.h>
#include <stdio.h>
#include <stdlib.h>
#include <string>
#include <map>
#include <vector>
#include <tinyxml2.h>
#include <Box2D/Box2D.h>
#include "GameDebug.hpp"
#include "Global.h"

#pragma comment(lib, "tinyxml2.lib")

using namespace std;

#include "Input.hpp"
#include "MessageDispatch.hpp"
#include "GameUtils.hpp"
#include "RelativeSpace.hpp"
#include "AnimatedSprite.hpp"
#include "GameObject.h"
#include "ItemDrop.hpp"
#include "Entity.hpp"
#include "SpawnManager.hpp"
#include "Box.hpp"
#include "Game.hpp"
#include "HUD.hpp"

HUD::HUD()
{
}


HUD::~HUD()
{
}

void HUD_GridPanel::AddObject(std::string elem_name, HUDObject obj)
{
	this->elements[elem_name] = obj;
}

void HUD_GridPanel::DrawPanel(int x, int y)
{
	int posx = 0, posy = 0;
	std::map<std::string, HUDObject>::iterator collection;

	for (collection = this->elements.begin(); collection != this->elements.end(); collection++) {
		SDL_Rect pos = { 0,0,0,0 };
		pos.x = x + (collection->second.column > 0 ? (this->width / this->columns)*collection->second.column : 0) + collection->second.localX;
		pos.y = y + (collection->second.row > 0 ? (this->height / this->rows)*collection->second.row : 0) + collection->second.localY;
		pos.w = collection->second.sprites->animclips[0].w;
		pos.h = collection->second.sprites->animclips[0].h;

		collection->second.sprites->Animate(pos, 0, NULL, SDL_FLIP_NONE, NULL);
	}
}

void HUD_FlowPanel::AddObject(HUDObject obj)
{

	this->elements.push_back(obj);
}

void HUD_FlowPanel::DrawPanel(int x, int y)
{
	int posx = 0, posy = 0;
	std::vector<HUDObject>::iterator collection;

	int tallestObject = 0;
	int indexX = 0;
	int indexY = 0;
	for (collection = this->elements.begin(); collection != this->elements.end(); collection++) {
		SDL_Rect pos = { 0,0,0,0 };
		if (indexX + this->spacingX + collection->sprites->animclips[0].w > x+this->width) {
			indexY += tallestObject+spacingY;
			indexX = x;
			pos.x = x+spacingX;
		}else{

			pos.x = x+indexX+spacingX;
			indexX += collection->sprites->animclips[0].w + spacingX;
		}
		if (tallestObject < collection->sprites->animclips[0].h) {
			tallestObject = collection->sprites->animclips[0].h;
		}
		pos.x += collection->localX;
		pos.y = y + indexY + collection->localY;
		pos.w = collection->sprites->animclips[0].w;
		pos.h = collection->sprites->animclips[0].h;

		collection->sprites->Animate(pos, 0, NULL, SDL_FLIP_NONE, NULL);
	}
}













/* HUD EFFECTS */
#define HUD_EFFECTS

std::map<std::string, HUDObject> registeredEffects;
std::map<std::string, bool> registeredEffectsComplete;
void HUD_ANIM_TransitionDown(HUDObject* h_Obj) {

	if (registeredEffectsComplete[h_Obj->e_ID] == false) {
		if (h_Obj->localY < 400) {
			h_Obj->localY += 10;
		}
		else {
			registeredEffectsComplete[h_Obj->e_ID] = true;
			h_Obj->localY = 0;
		}
	}
	registeredEffects[h_Obj->e_ID] = *h_Obj;
}

void HUD_ANIM_DMGNO_TransitionUp(HUDObject* h_Obj) {

	if (registeredEffectsComplete[h_Obj->e_ID] == false) {
		if (h_Obj->localY > -50) {
			h_Obj->localY -= 5;
		}
		else {
			registeredEffectsComplete[h_Obj->e_ID] = true;
		}
	}
	registeredEffects[h_Obj->e_ID] = *h_Obj;
}

void HUD::readMouseInput()
{

	SDL_GetMouseState(&MH_mouseX, &MH_mouseY);
}

void HUD_Button::Present(SDL_Rect pos)
{
	pos.w = this->normal.animclips[0].w;
	pos.h = this->normal.animclips[0].h;

	int w = pos.w;
	int h = pos.h;
	if ((MH_mouseX > pos.x && MH_mouseX < pos.x + w) && (MH_mouseY > pos.y && MH_mouseY < pos.y + h)) {
		if (MH_clicked == true) {
			this->pressed.Animate(pos, 0, NULL, SDL_FLIP_NONE, nullptr);
			if (this->clicked == false) {
				this->clicked = true;
				if (this->methods["mouseDown"] != nullptr) {
					this->methods["mouseDown"](this);
				}
			}
		}
		else {
			if (this->entered == false) {
				this->entered = true;
				if (this->methods["mouseEnter"] != nullptr) {
					this->methods["mouseEnter"](this);
				}
			}

			if (this->clicked == true) {
				if (this->methods["mouseUp"] != nullptr) {
					this->methods["mouseUp"](this);
				}

				this->clicked = false;
			}

			this->hover.Animate(pos, 0, NULL, SDL_FLIP_NONE, nullptr);
		}
	}
	else {
		
		if (MH_clicked == false) {
			if (this->entered == true) {
				if (this->methods["mouseLeave"] != nullptr) {
					this->methods["mouseLeave"](this);
				}

				this->entered = false;
			}


			if (this->clicked == true) {
				if (this->methods["mouseUp"] != nullptr && this->registerOutOfBoundUp) {
					this->methods["mouseUp"](this);
				}

				this->clicked = false;
			}

			this->clicked = false;
		}
		if (!this->registerOutOfBoundUp && this->entered) {
			this->entered = false;
		}
		this->normal.Animate(pos, 0, NULL, SDL_FLIP_NONE, nullptr);
	}

}

void HUD_Button::BindAction(std::string method, void(*methodFunc)(void* context), void* context)
{
	this->methods[method] = methodFunc;
}

HUD_Button::HUD_Button()
{
	
}

HUD_Button::~HUD_Button()
{

}

void HUD_TextBlock::AddWObject(std::string word)
{
	//Convert word to objects, add them to elements
	size_t i = 0;
	for (i = 0; i < word.length(); i++) {
		std::string itemNo = "UILetter.";
		itemNo += word[i];

		HUDObject hObj;
		hObj.sprites = &HUDElements[itemNo];
		this->elements.insert(this->elements.end(), hObj);
	}

	this->mapping_wordcount[this->mapping_wordcount.size()] = i;
	this->changed = true;
}

void HUD_TextBlock::DrawPanel(int x, int y)
{
	int posx = 0, posy = 0;
	std::vector<HUDObject>::iterator collection;

	int tallestObject = 0;
	int indexX = x;
	int indexY = 0;
	int wcIndex = 0;
	int wcLIndex = 0;
	int indexa = 0;
	bool tooBig = false;
	for (collection = this->elements.begin(); collection != this->elements.end(); collection++) {
		SDL_Rect pos = { 0,0,0,0 };

		if (wcLIndex >= this->mapping_wordcount[wcIndex]) {
			wcLIndex = 0;
			wcIndex++;
			indexX += this->wordSpacing;
			int wordwidth = (indexX + ((this->letterSpacing + collection->sprites->animclips[0].w) * this->mapping_wordcount[wcIndex]));
			tooBig = wordwidth > x + this->width;
		}
		
		if (tooBig) {
			indexY += tallestObject + this->lineSpacing;
			indexX = x;
			pos.x = x + this->letterSpacing+indexX;

			if (tooBig) {
				tooBig = false;
			}
		}
		else {

			pos.x = x + indexX + this->letterSpacing;
		}
		indexX += collection->sprites->animclips[0].w + letterSpacing;
		if (tallestObject < collection->sprites->animclips[0].h) {
			tallestObject = collection->sprites->animclips[0].h;
		}
		pos.x += collection->localX;
		pos.y = y + indexY + collection->localY;
		pos.w = collection->sprites->animclips[0].w;
		pos.h = collection->sprites->animclips[0].h;

		collection->sprites->Animate(pos, 0, NULL, SDL_FLIP_NONE, NULL);


		wcLIndex++;
		indexa++;
	}
}
