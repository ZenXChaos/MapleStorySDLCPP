#### MapleSDL :: Creating a MapleStory based MMORPG for learning purposes.

Disclaimer: All content has been downloaded from MapleSimulator, and is perfectly fine to user for learning and fan-based use. All assets are property of Nexon and it's partners. No commercial use of the `art`, please!

### Features

* Art

> Animation

* Mobs

> Basic AI

> State Machine

> Roaming

> Dynamic loading ( xml; .zenx files )

* Player

> Movement

* Game Logic

> Dynamic content loading

> Spawn Manager

---

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


This project is very active, and contributions are welcome! (:

[Follow me on Twitter](https://twitter.com/ZenXChaos)

[Follow UberSnipDev on Twitter](https://twitter.com/UberSnipDev)