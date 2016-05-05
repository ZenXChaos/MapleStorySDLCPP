#include "LUtil.h"
#include <IL/il.h>
#include <IL/ilu.h>
#include "LTexture.h"

#include "MessageDispatch.hpp"
#include "GameUtils.hpp"
#include "RelativeSpace.hpp"
#include "ItemDrop.hpp"
#include "Input.hpp"
#include "Entity.hpp"
#include "SpawnManager.hpp"
#include "GameMap.hpp"
#include "Game.hpp"
#include "Camera.hpp"

//VBO rendered texture
LTexture gVBOTexture;
LTexture gVBOTexture2;


int tick = 0;

std::map<std::string, Entity*> MobList;

Game game;
Player entity(&game.spawn_manager.spawned);
GameMap map;
LFRect mapPos;

Entity mushy;

void keyPressed(unsigned char key, int x, int y) {
	entity.playerInput.KeyDown(key);
}

void keyUp(unsigned char key, int x, int y) {
	entity.playerInput.KeyUp(key);
}

void keySpecialPressed(int key, int x, int y) {
	if (key == GLUT_KEY_LEFT) {
		keyPressed('<', 0, 0);
	}
	else if (key == GLUT_KEY_RIGHT) {
		keyPressed('>', 0, 0);
	}
}

void keySpecialUp(int key, int x, int y) {
	if(key == GLUT_KEY_LEFT) {
		keyUp('<', 0, 0);
	}
	else if (key == GLUT_KEY_RIGHT) {
		keyUp('>', 0, 0);
	}
}

void Core() {
	map.DrawMap(mapPos);
	entity.Draw();
	//entity.ManageState();
	game.ManageMobPool();
	//game.ManageMapObjects();

	//MobList["mush"]->Draw();
	//mushy.Draw();

	if (entity.playerInput.IsKeyPressed('<')) {
		entity.Walk(Left);
	}
	else if (entity.playerInput.IsKeyPressed('>')) {
		entity.Walk(Right);
	}
	else {
		entity.Station();
	}

}

bool initGL()
{
    //Initialize GLEW
    GLenum glewError = glewInit();
    if( glewError != GLEW_OK )
    {
        printf( "Error initializing GLEW! %s\n", glewGetErrorString( glewError ) );
        return false;
    }

    //Make sure OpenGL 2.1 is supported
    if( !GLEW_VERSION_2_1 )
    {
        printf( "OpenGL 2.1 not supported!\n" );
        return false;
    }

    //Set the viewport
    glViewport( 0, 0, SCREEN_WIDTH, SCREEN_HEIGHT );

    //Initialize Projection Matrix
    glMatrixMode( GL_PROJECTION );
    glLoadIdentity();
    glOrtho( 0.0, SCREEN_WIDTH, SCREEN_HEIGHT, 0.0, 1.0, -1.0 );

    //Initialize Modelview Matrix
    glMatrixMode( GL_MODELVIEW );
    glLoadIdentity();

    //Initialize clear color
    glClearColor( 0.f, 0.f, 0.f, 1.f );

    //Enable texturing
    glEnable( GL_TEXTURE_2D );

    //Set blending
    glEnable( GL_BLEND );
    glDisable( GL_DEPTH_TEST );
    glBlendFunc( GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA );

    //Check for error
    GLenum error = glGetError();
    if( error != GL_NO_ERROR )
    {
        printf( "Error initializing OpenGL! %s\n", gluErrorString( error ) );
        return false;
    }

    //Initialize DevIL and DevILU
    ilInit();
    iluInit();
    ilClearColour( 255, 255, 255, 000 );

    //Check for error
    ILenum ilError = ilGetError();
    if( ilError != IL_NO_ERROR )
    {
        printf( "Error initializing DevIL! %s\n", iluErrorString( ilError ) );
        return false;
    }

	game.MobList = &MobList;

	//MobList["mush"] = new Entity();
	//MobList["mush"]->animations["idle"].AddSprite("content\\maps\\hennesys\\map01.png", 0.1f);
	//Then init game
	game.SetMainPlayer(&entity);
	//game.LoadItemDrops();
	game.LoadMobList();
	game.LoadPlayerAnims(&entity);
	game.InitSpawnManager();
	entity.SetPositionY(190);
	mapPos.w = SCREEN_WIDTH;
	mapPos.h = SCREEN_HEIGHT;
	mapPos.x = 0;
	mapPos.y = -407;

	glutKeyboardFunc(keyPressed);
	glutKeyboardUpFunc(keyUp); 

	glutSpecialFunc(keySpecialPressed);
	glutSpecialUpFunc(keySpecialUp);
    return true;
}

bool loadMedia()
{
	
	/*if (!gVBOTexture.loadTextureFromFile("opengl.png"))
	{
		printf("Unable to load OpenGL texture!\n");
		return false;
	}

	if (!gVBOTexture2.loadTextureFromFile("opengl.png"))
	{
		printf("Unable to load OpenGL texture!\n");
		return false;
	}*/

	
	return true;
}

void update(int val)
{
	tick++;

}

void render()
{	
    //Clear color buffer
    glClear( GL_COLOR_BUFFER_BIT );

    //Initialize modelview matrix
    glLoadIdentity();

    //Render textured quad using VBOs
	//gVBOTexture.render( ( SCREEN_WIDTH - gVBOTexture.imageWidth() ) / 2.f, ( SCREEN_HEIGHT - gVBOTexture.imageHeight() ) / 2.f );
	//gVBOTexture2.render((SCREEN_WIDTH - gVBOTexture.imageWidth()) / 2.f, (SCREEN_HEIGHT - gVBOTexture.imageHeight()) / 2.f);
	Core();
    //Update screen
	glutSwapBuffers();
	game.spawn_manager.ManagePool(tick);
}
