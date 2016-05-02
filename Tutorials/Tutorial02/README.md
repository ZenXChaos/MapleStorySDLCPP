### Loading images

In this tutorial, we will learn how to load an image using *SDL_image* which we installed in [Tutorial 01](http://gamedev.ubersnip.com/index.php?title=Tutorials_Chapter1_-_Setting_Up_CPP_SDL).

For starters, we need to create two files. *Sprite.hpp* and *Sprite.cpp* . We will do most of the image handling in the *Sprite* class.

Let's define our class in *Sprite.hpp*, then include necessary headers in *Sprite.cpp* .

<pre>
<nowiki>
#ifndef SPRITE_H
#define  SPRITE_H
class Sprite
{
public:
	Sprite();
	virtual ~Sprite();
};
#endif
</nowiki>
</pre>

When displaying a sprite, there are several things that the developer should keep in mind. Here are a few things.

* Sprite file location
* Sprite Format (png,bmp,etc)
* Sprite Width
* Sprite Height
* Intended sprite XY location

Let's define the variables necessary to store this information.

<pre>
<nowiki>
public:
int w = 0;
int h = 0;
 
private:
SDL_Texture* texture = nullptr;
</nowiki>
</pre>

Your class should now look like the following:

<pre>
<nowiki>
#ifndef SPRITE_H
#define  SPRITE_H
class Sprite
{

	SDL_Texture* texture;
public:
	int w = 0;
	int h = 0;

	Sprite();
	virtual ~Sprite();
};
#endif
</nowiki>
</pre>

How about loading the texture? Let's create a function for that! *bool LoadTexture(std::string filename, SDL_Renderer* gRenderer)* .

We also need to create a function to draw the sprite after it is loaded. *void Draw(SDL_Rect pos* .

### Loading the texture

*Texture.cpp*

<pre>
<nowiki>
bool Sprite::LoadTexture(std::string path, SDL_Renderer* gRenderer)
{
	SDL_Texture* fTexture = nullptr;

	//Load image
	SDL_Surface* loadedSurface = IMG_Load(path.c_str());
	if (loadedSurface == nullptr)
	{
		printf("Failed to load image %s! SDL_image Error: %s\n", path.c_str(), IMG_GetError());
		return false;
	}
	else
	{
		//Create texture from surface pixels
		this->texture = SDL_CreateTextureFromSurface(gRenderer, loadedSurface);
		if (this->texture == nullptr)
		{
			printf("Unable to create texture from %s! SDL Error: %s\n", path.c_str(), SDL_GetError());
			return false;
		}

		//Free old loaded surface
		SDL_FreeSurface(loadedSurface);
	}

	SDL_QueryTexture(this->texture, NULL, NULL, &this->w, &this->h);

	return true;
}
</nowiki>
</pre>

Essentially, what we just did was:

* Loaded an image -- storing it in a temporary texture.
* Create a texture from surface pixels.
* Finally, store width and height of texture.

### Drawing the sprite

*Texture.cpp*

Let's create our *Draw()* function!

<pre>
<nowiki>
void Sprite::Draw(SDL_Rect pos, SDL_Renderer* gRenderer)
{
	SDL_Rect newPos = pos;
	newPos.w = this->w;
	newPos.h = this->h;
	SDL_RenderCopy(gRenderer, this->texture, NULL, &newPos);
}
</nowiki>
</pre>

Here is what's happening in our draw function.

* We create a new SDL_Rect, copying the pos variable.
* We then set the newPos width and height according to the textures width and height.
* Finally, render to screen.


Our Sprite class is all finished. All we have to do now is create a reference, include proper headers, and call *.Draw()* .

### Displaying the sprite

In our main function, there are a few things we have to first do.

* Init PNG

<pre>
<nowiki>
//Initialize PNG loading
int imgFlags = IMG_INIT_PNG;
if (!(IMG_Init(imgFlags) & imgFlags))
{
	printf("SDL_image could not initialize! SDL_image Error: %s\n", IMG_GetError());
}
</nowiki>
</pre>

* Before the loop, we need to add a sprite.

<pre>
<nowiki>
Sprite shroom;
shroom.LoadTexture("content\\shroom.png", m_gRenderer);
</nowiki>
</pre>

* Call it's .Draw() in the game loop.

Please note that you must clear the screen, and present the new content as shown below.

<pre>
<nowiki>
SDL_RenderClear(m_gRenderer);
shroom.Draw({ 0 }, m_gRenderer);
SDL_RenderPresent(m_gRenderer);
</nowiki>
</pre>


Here is the full source code:

<pre>
<nowiki>
//main.cpp
#include &lt;SDL.h&gt;
#include &lt;SDL_image.h&gt;
#include &lt;stdio.h&gt;
#include &lt;string&gt;

#include "Sprite.hpp"

#undef main

//SCREEN WIDTH / HEIGHT
const int SCREEN_WIDTH = 800;
const int SCREEN_HEIGHT = 400;
int main(int argc, char* argv) {
	if (SDL_Init(SDL_INIT_EVERYTHING) < 0)
	{
		printf("Could not initialize SDL! SDL_Error: %s\n", SDL_GetError());
	}

	SDL_Window* window = nullptr;
	SDL_Renderer* m_gRenderer = nullptr;

	window = SDL_CreateWindow("SDL Tutorial", 50, 50, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN);
	m_gRenderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);

	//Initialize renderer color
	SDL_SetRenderDrawColor(m_gRenderer, 0xFF, 0xFF, 0xFF, 0xFF);

	//Initialize PNG loading
	int imgFlags = IMG_INIT_PNG;
	if (!(IMG_Init(imgFlags) & imgFlags))
	{
		printf("SDL_image could not initialize! SDL_image Error: %s\n", IMG_GetError());
	}

	if (window == nullptr)
	{
		printf("Failed to create window! SDL_Error: %s\n", SDL_GetError());
		return 0;
	}

	Sprite shroom;
	shroom.LoadTexture("content\\shroom.png", m_gRenderer);

	bool running = true;
	while (running) {

		SDL_Event event;
		while (SDL_PollEvent(&event)) {
		}
		//Clear screen
		SDL_RenderClear(m_gRenderer);
		shroom.Draw({ 0 }, m_gRenderer);
		SDL_RenderPresent(m_gRenderer);
	}

	SDL_DestroyWindow(window);
	return 1;
}
</nowiki>
</pre>


<pre>
<nowiki>
//Sprite.cpp
#include &lt;SDL.h&gt;
#include &lt;SDL_image.h&gt;
#include &lt;stdio.h&gt;
#include &lt;string&gt;
#include "Sprite.hpp"



bool Sprite::LoadTexture(std::string path, SDL_Renderer* gRenderer)
{

	//Load image
	SDL_Surface* loadedSurface = IMG_Load(path.c_str());
	if (loadedSurface == nullptr)
	{
		printf("Failed to load image %s! SDL_image Error: %s\n", path.c_str(), IMG_GetError());
		return false;
	}
	else
	{
		//Create texture from surface pixels
		this->texture = SDL_CreateTextureFromSurface(gRenderer, loadedSurface);
		if (this->texture == nullptr)
		{
			printf("Unable to create texture from %s! SDL Error: %s\n", path.c_str(), SDL_GetError());
			return false;
		}

		SDL_QueryTexture(this->texture, NULL, NULL, &this->w, &this->h);

		//Free old loaded surface
		SDL_FreeSurface(loadedSurface);
	}

	return true;
}

void Sprite::Draw(SDL_Rect pos, SDL_Renderer* gRenderer)
{
	SDL_Rect newPos = pos;
	newPos.w = this->w;
	newPos.h = this->h;
	SDL_RenderCopy(gRenderer, this->texture, NULL, &newPos);
}

Sprite::Sprite()
{
}


Sprite::~Sprite()
{
	
}
</nowiki>
</pre>

<pre>
<nowiki>
//Sprite.hpp
#ifndef SPRITE_H
#define  SPRITE_H
class Sprite
{

	SDL_Texture* texture = nullptr;
public:
	int w = 0;
	int h = 0;

	//Load texture from a file
	bool LoadTexture(std::string path, SDL_Renderer* gRenderer);

	//Draw texture to screen
	void Draw(SDL_Rect pos, SDL_Renderer* gRenderer);

	Sprite();
	virtual ~Sprite();
};
#endif
</nowiki>
</pre>