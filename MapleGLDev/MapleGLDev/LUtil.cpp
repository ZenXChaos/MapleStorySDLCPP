#include "LUtil.h"
#include <IL/il.h>
#include <IL/ilu.h>
#include "LTexture.h"

#include "MessageDispatch.h"
#include "GameUtils.h"
#include "RelativeSpace.h"
#include "ItemDrop.hpp"
#include "Entity.h"
#include "SpawnManager.h"
#include "GameMap.h"
#include "Game.h"

//VBO rendered texture
LTexture gVBOTexture;
LTexture gVBOTexture2;


int tick = 0;

Game game;
Player entity(&game.spawn_manager.spawned);
GameMap map;
LFRect mapPos;

void Core() {
	map.DrawMap({ 0,0,0,0 });
	entity.ManageState();
	entity.Draw();
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

	//Then init game
	game.SetMainPlayer(&entity);
	game.LoadItemDrops();
	game.LoadMobList();
	game.InitSpawnManager();

	mapPos.w = 1387;
	mapPos.h = 907;
	mapPos.x = 0;
	mapPos.y = -407;
	map.InitMap("content\\maps\\hennesys\\map01.png", mapPos);
	//entity.SetPositionY(0);

    return true;
}

AnimatedVBO testvb;
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
	entity.animations["idle"];
	//testvb.AddSprite("content\\mobs\\mush\\idle\\stand_0.png", 0.1);
	game.LoadPlayerAnims(&entity);
	//entity.animations["idle"].AddSprite("content\\player\\idle\\stand1_0.png", 0.1);
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
	entity.Draw();
	Core();
    //Update screen
    glutSwapBuffers();
}
