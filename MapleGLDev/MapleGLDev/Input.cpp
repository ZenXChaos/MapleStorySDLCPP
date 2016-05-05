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

#include "GameUtils.hpp"
#include "RelativeSpace.hpp"
#include "MessageDispatch.hpp"
#include "ItemDrop.hpp"
#include "Input.hpp"
#include "Entity.hpp"
#include "SpawnManager.hpp"
#include "Game.hpp"

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