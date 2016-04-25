#include "LUtil.h"
#include <IL/il.h>
#include <IL/ilu.h>
#include "LTexture.h"

#include "RelativeSpace.h"
#include "Entity.h"

extern AnimatedVBO mushmob;
Entity mobmush;

void InitGameMGR() {

	mobmush.animations->insert(std::pair<std::string, AnimatedVBO>("idle",mushmob));
}

void GameRun() {
	mobmush.Draw();
}