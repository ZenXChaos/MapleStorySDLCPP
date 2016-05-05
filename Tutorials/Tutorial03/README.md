### Animating a sprite

In this tutorial, we will learn how to '''animate''' the sprite we displayed in the tutorial: [http://gamedev.ubersnip.com/index.php?title=Displaying_a_sprite '''Displaying a sprite'''].

Let's first create our source and header files. '''AnimatedSprite.cpp''' and '''AnimatedSprite.h'''. This is what our initial '''AnimatedSprite.cpp''' should look like.

<pre>
<nowiki>
#include <SDL.h>
#include <SDL_image.h>
#include <stdio.h>
#include <string>
#include "Sprite.hpp"
#include "AnimatedSprite.h"

AnimatedSprite::AnimatedSprite()
{
}


AnimatedSprite::~AnimatedSprite()
{
}
</nowiki>
</pre>


'''AnimatedSprite.h'''

<pre>
<nowiki>
#ifndef ANIMATED_SPRITE_H
#define ANIMATED_SPRITE_H
class AnimatedSprite
{
public:
	AnimatedSprite();
	virtual ~AnimatedSprite();
};

#endif
</nowiki>
</pre>

Additionally, in '''main.cpp'', we should include '''AnimatedSprite.h''' and '''<vector>'''.

<pre>
<nowiki>
#include <SDL.h>
#include <SDL_image.h>
#include <stdio.h>
#include <string>
#include <vector>

#include "Sprite.hpp"
#include "AnimatedSprite.h"
</nowiki>
</pre>


Now, let's add a few variables to our '''AnimatedSprite''' class.

### Variables

<pre>
<nowiki>
private:
float frame = 0.0f;

public:

float maxFrames = 0.9f;
float delta = 0.001f;

std::vector<Sprite> animFrames;
</nowiki>
</pre>

'''frame''': Keeps track of the current frame number we are at.

'''maxFrames''': Maximum number of frames in an animation.

'''delta''': This manages the speed an animation plays at.

'''animframes''': Individual sprite frames from class '''Sprite'''.


### Methods

<pre>
<nowiki>
private:

public:

void AddSprite(Sprite sprite);
void Step();
void Animate(SDL_Rect pos, SDL_Renderer* gRenderer);
</nowiki>
</pre>

'''AddSprite()''': Adds a sprite to the animation stack.

'''Step()''': Goes to next animation in stack.

'''Animate()''': Animates the animation (Displays texture at x '''frame''').


Let's define our functions now.

'''AnimatedSprite.cpp''':


'''void AnimatedSprite::AddSprite(Sprite sprite)'''

<pre>
<nowiki>
void AnimatedSprite::AddSprite(Sprite sprite){
	this->animFrames.insert(this->animFrames.end(), sprite);
	this->maxFrames += 1.0f;
}
</nowiki>
</pre>

This inserts a sprite into our '''animFrames''' container. We also need to increase the '''maxFrames''' count by one.


'''void AnimatedSprite::Step()'''
<pre>
<nowiki>
void AnimatedSprite::Step(){
	this->frame += this->delta;

	if (this->frame >= this->maxFrames - 1.0f) {
		this->frame = 0.0f;
	}
}
</nowiki>
</pre>

What happens here is the '''frame''' number is increased by '''delta'''. If the maximum number of frames is reached or exceeded, reset '''frame''' number to zero.


'''Animate()'''
<pre>
<nowiki>
void AnimatedSprite::Animate(SDL_Rect pos, SDL_Renderer* gRenderer){
	this->animFrames[static_cast<int>(this->frame)].Draw(pos, gRenderer);
}
</nowiki>
</pre>

This simply draws the texture at whatever '''frame''' is equal to. Notice static_cast<int>(), which just converts from float to int in this instance.

### Finishing up

Our class is all setup, and all we need to do now is load our sprites. Let's go back to '''main.cpp'''.

Change:

<pre>
<nowiki>
Sprite shroom;
shroom.LoadTexture("content\\shroom.png", m_gRenderer);
</nowiki>
</pre>

to

<pre>
<nowiki>

Sprite shroom_sprite01;
shroom_sprite01.LoadTexture("content\\mush\\walk\\move_0.png", m_gRenderer);

Sprite shroom_sprite02;
shroom_sprite02.LoadTexture("content\\mush\\walk\\move_1.png", m_gRenderer);

Sprite shroom_sprite03;
shroom_sprite03.LoadTexture("content\\mush\\walk\\move_2.png", m_gRenderer);

Sprite shroom_sprite04;
shroom_sprite04.LoadTexture("content\\mush\\walk\\move_3.png", m_gRenderer);

AnimatedSprite shroom;
shroom.AddSprite(shroom_sprite01);
shroom.AddSprite(shroom_sprite02);
shroom.AddSprite(shroom_sprite03);
shroom.AddSprite(shroom_sprite04);
shroom.delta = 0.005f;

</nowiki>
</pre>

and also change 

<pre>
<nowiki>
shroom.Draw({ 0 }, m_gRenderer);
</nowiki>
</pre>

to

<pre>
<nowiki>
shroom.Animate({ 0 }, m_gRenderer);
</nowiki>
</pre>