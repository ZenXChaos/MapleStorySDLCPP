#### MapleSDL :: Creating a MapleStory based MMORPG for learning purposes.

Disclaimer: All content has been downloaded from MapleSimulator, and is perfectly fine to use for learning and fan-based use. Only GMS discourages using it's assets, and therefor, no GMS assets are included in this project. All assets are property of Nexon and it's partners. No commercial use of the `art`, please! All sprites have been downloaded from https://maplesimulator.com/. Please be mindful that this project is NOT a private server, and does not act like so in any form. This project does not even follow MapleStory's plot -- and does not require it to be installed. MapleStory just happens to be the most open with their art, and I happen to be a long time mega-fan of MapleStory. Again, this is for learning how to construct your own MMORPG. (:

[Wiki Documentation](https://github.com/ZenXChaos/MapleStorySDLCPP/wiki)

### Features

* Physics
 
> Box2D (Experimental)

* Art

> Animation

* Mobs

> Item Drops

> Basic AI

> State Machine

> Roaming

> Dynamic loading ( xml; .zenx files )

* Player

> Movement

> Attacking

> State Machine

* Game Logic

> Basic Map

> Dynamic content loading

> Spawn Manager

> MessageDispatching (Player to Mob) entity communication.

---

#### Controls

Movement : Left / Right Arrow Keys

Attack: C

---

![SS](https://raw.githubusercontent.com/ZenXChaos/MapleStorySDLCPP/master/SS.gif)

![SS2](https://raw.githubusercontent.com/ZenXChaos/MapleStorySDLCPP/master/SS2.gif)

##### Loading Mobs

Mob data is stored at `data\mobs\mobs.zenx`. Each `mob` node contains all data related to a specific mob.

The following attributes are required for binding an animation sprite sheet.

> name="walk" // Animation name

> sprite="mobs\mush\walking\walking01_left.png" // Spritesheet file location : Starting at root of folder

> max_frames="3" // Number of animations the spritesheet has

> delta="0.1" // How fast to animate the sprite?

> sprite_width="100" // Width of individual sprites

> sprite_height="100" // Heigh of individual sprites

> yfactorup="0" //Shift sprite up by 0 on each frame

Sprites should all be on one row. Multi-row spritesheets are not supported at this time.


### Code has been tested only on Windows Visual Studio 2015 - On windows 10. You may need to retarget to get things working!

This project is very active, and contributions are welcome! (:

[Follow me on Twitter](https://twitter.com/ZenXChaos)

[Follow UberSnipDev on Twitter](https://twitter.com/UberSnipDev)