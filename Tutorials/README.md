### Initializing SDL

First, we have to of course create our main entry point. Create a file '''main.cpp'''.

To get things started, we have to include SDL2.

<pre>
<nowiki>
	#include &lt;SDL.h&gt;
	#include &lt;stdio.h&gt;
</nowiki>
</pre>

and then create our main application entry point - as well as define our intended screen width/height.

<pre>
<nowiki>

//SCREEN WIDTH / HEIGHT
const int SCREEN_WIDTH = 800;
const int SCREEN_HEIGHT = 400;
int main(int argc, char* argv) {

	return 1;
}
</nowiki>
</pre>

Ok! So we've gotten that out of the way, and now we want to actually start playing with SDL.

The first thing we have to do in SDL, is initialize it! For the sake of simplicity, we will just initialize everything in SDL.

<pre>
<nowiki>
SDL_Init(SDL_INIT_EVERYTHING);
</nowiki>
</pre>

### Creating a window

The bare minimum for running a SDL draw-able surface requires '''SDL_Renderer''' and '''SDL_Window'''. Let's add these two variables to our '''main''' function like so:

<pre>
<nowiki>
int main(int argc, char* argv) {
	if (SDL_Init(SDL_INIT_EVERYTHING) < 0)
	{
		printf("Could not initialize SDL! SDL_Error: %s\n", SDL_GetError());
	}

	SDL_Window* window = nullptr;
	SDL_Renderer* m_gRenderer;


	return 1;
}
</nowiki>
</pre>


Assuming all went well, and you received no errors, we have to create the window.

<pre>
<nowiki>
int main(int argc, char* argv) {
	if (SDL_Init(SDL_INIT_EVERYTHING) < 0)
	{
		printf("Could not initialize SDL! SDL_Error: %s\n", SDL_GetError());
	}

	SDL_Window* window = nullptr;
	SDL_Renderer* m_gRenderer = nullptr;

	window = SDL_CreateWindow("SDL Tutorial", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN);

	if (window == nullptr)
	{
		printf("Failed to create window! SDL_Error: %s\n", SDL_GetError());
		return 0;
	}

	return 1;
}
</nowiki>
</pre>

Let's break down '''SDL_CreateWindow'''

'''Parameters'''
* Window Title: "SDL Tutorial"
* The X position the window starts up at.
* The Y position the window starts up at.
* The width you want the window to be.
* The height you want the window to be.
* The window style.

You should get the following window.

![SS](http://gamedev.ubersnip.com/images/a/aa/SDLTutorialCreatingAWindow.png)

'''Full Source Code'''

<pre>
<nowiki>
#include &lt;SDL.h&gt;
#include &lt;stdio.h&gt;

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

	window = SDL_CreateWindow("SDL Tutorial", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN);

	if (window == nullptr)
	{
		printf("Failed to create window! SDL_Error: %s\n", SDL_GetError());
		return 0;
	}

	return 1;
}
</nowiki>
</pre>

'''Common issues'''

Sometimes, with Visual Studio via NuGet, you will get a resolution error in reference to '''main()'''. Add the following after your SDL include if you get this error.

<pre>
<nowiki>
#undef main
</nowiki>
</pre>

It should look something like this

<pre>
<nowiki>
#include &lt;SDL.h&gt;
#include &lt;stdio.h&gt;

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

	window = SDL_CreateWindow("SDL Tutorial", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN);

	if (window == nullptr)
	{
		printf("Failed to create window! SDL_Error: %s\n", SDL_GetError());
		return 0;
	}

	return 1;
}
</nowiki>
</pre>

### The Game Loop

You probably noticed that your window closed almost immediately after being displayed. That's because we need a loop that lets the game know we are still running. Add this to the bottom of your '''main()''' function.

<pre>
<nowiki>
bool running = true;
while (running) {

	SDL_Event event;
	while (SDL_PollEvent(&event)) {

	}
}
</nowiki>
</pre>

'''SDL_Event''' handles events such as keyboard presses/releases, mouse input, joystick input, etc. We will learn more about this later.

With the applied changes, your code should look like the following.

<pre>
<nowiki>
#include &lt;SDL.h&gt;
#include &lt;stdio.h&gt;

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

	window = SDL_CreateWindow("SDL Tutorial", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN);

	if (window == nullptr)
	{
		printf("Failed to create window! SDL_Error: %s\n", SDL_GetError());
		return 0;
	}

	bool running = true;
	while (running) {

		SDL_Event event;
		while (SDL_PollEvent(&event)) {

		}
	}

	return 1;
}
</nowiki>
</pre>