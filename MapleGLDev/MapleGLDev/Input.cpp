#include "LUtil.h"
#include <IL/il.h>
#include <IL/ilu.h>
#include "LTexture.h"

#include <stdio.h>
#include <stdlib.h>
#include <string>
#include <map>
#include <vector>
#include <tinyxml2.h>

#pragma comment(lib, "tinyxml2.lib")

using namespace std;

#include "GameUtils.h"
#include "RelativeSpace.h"
#include "MessageDispatch.h"
#include "ItemDrop.hpp"
#include "Input.h"
#include "Entity.h"
#include "SpawnManager.h"
#include "Game.h"

void Input::KeyDown(unsigned char key) {
	this->keysDown[key] = true;
	this->keysUp[key] = false;
}

void Input::KeyUp(unsigned char key) {
	this->keysDown[key] = false;
	this->keysUp[key] = true;

}

bool Input::IsKeyPressed(unsigned char key) {
	return this->keysDown[key];
}