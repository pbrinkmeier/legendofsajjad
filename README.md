# Legend of Sajjad 
Minigame.

# How to build
There are binaries available for Windows (can be found in the subreddit), MacOS and Linux users should be able to compile the code themselves

How to build on Linux (should be similar for MacOS)
- install the necessary libraries via your package manager, these are the following:
	- git
	- cmake
	- sdl2
	- sdl2_image
	- sdl2_mixer
- git clone this project or download as zip and unzip in any directory
- create a new folder named "bin"
- cd into that folder and call "cmake .."
- call "make"
- you should find a new binary called "legendofsajjad", run this from the terminal using "./legendofsajjad" or from your file viewer
- if you get any errors, confirm that you have the appropriate libraries installed

# TODO
## Code cleanup / enhancement tasks
- add "spritesheet" and "animation" abstraction classes to ease adding new sprites
- resource "server" loading the spritesheets, sounds etc.

## Maybe adding this in the future, if I get the time
- let player move freely even if the camera can't move anymore (borders/corners of the map)
- powerups (new weapons, speed boost, invincibility)
- different floor tiles/worldmaps
- different boss/penguin AIs to randomly choose from
- font rendering to show important (or plain silly) information (fps, deaths, shot penguins etc.)

# Known bugs
- sometimes sounds like the boss death sound are skipped for some reason

# Special thanks
- Thanks to 8BitUniverse whose LoZ and Take On Me 8 bit renditions I used due to my own lack of musical talent. All rights to these tracks belong to them.
