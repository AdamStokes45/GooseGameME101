//GAMEPLAY HEADER
//gooseEscapeGamePlay.hpp

#ifndef GOOSE_ESCAPE_GAMEPLAY
#define GOOSE_ESCAPE_GAMEPLAY
#include "gooseEscapeUtil.hpp"
#include "gooseEscapeActor.hpp"
#include "gooseEscapeConsole.hpp"

/*
    This file is all about the game world.  You will modify this to add
    constants and function prototypes.  Modify gooseGamePlay.cpp to
	actually add functionality.
*/

/*
    Declare constants to indicate various game world features in the board
    array.  Modify them to fit what you would like to do in the game.
*/

// Going further:  Learn how to use an enum for these values
const int EMPTY = 0;
const int WALL = 1;
const int WINNER = 2;
const int EASY = 3;
const int MEDIUM = 4;
const int HARD = 5;

const int MAP_X = 80;
const int MAP_Y = 21;
const int LEVEL_SELECT_SIZE = 3;
const int INFO_SIZE = 4;



/*
    A few examples of characters both for actors and for the game board itself
    are shown.
*/
//display characters
const int PLAYER_CHAR = int('@');
const int MONSTER_CHAR = int('G');
const int WALL_CHAR = int('o');
const int WIN_CHAR = int('%'); //% sign, a special character used in the ancient game "Rogue"
const int EASY_CHAR = int('E');
const int MEDIUM_CHAR = int('M');
const int HARD_CHAR = int('H');
const int POWER_1 = int('1');
const int POWER_2 = int('2');

/*
Colours have the format "brightness hue" or "hue", e.g. "white" "light green"
Values for brightness are: lightest, lighter, light, dark, darker, darkest
Hue may be specified in many ways, see http://foo.wyrd.name/en:bearlibterminal:reference
  hue names are: grey, gray, red, flame, orange, amber, yellow, lime, chartreuse
                 green, sea, turquoise, cyan, sky, azure, blue, han, violet,
                 purple, fushsia, magenta, pink, crimson, transparent
*/
/*
  Declare a character array with the colour name, the size is set based on the
  number of character used to initialize the array.
  
  DRAW_COLOUR is already defined as white.
*/

const char PLAYER_COLOUR[] = "light sky";
const char GOOSE_COLOUR[] = "yellow";

/*
	Print the game world
*/
// function header

/*
    Do something when the goose captures the player
*/
bool captured(Actor const & player, Actor const & monster);

/*
    Move the player to a new location based on the user input
*/
void movePlayer(int key, Actor & player, int map[MAP_X][MAP_Y],bool & win, int & powerup, int & uses);

/*
    What other functions do you need to make the game work?  What can you add
    to the basic functionality to make it more fun to play?
*/

void printBoard(int map[MAP_X][MAP_Y]);

void moveGoose(Actor & monster, Actor & player, int map[MAP_X][MAP_Y]);

void moveStarter(int key, Actor & starter, int map[MAP_X][MAP_Y], int & level_selected);

void generateRandomArea(int amount_lower, int amount_range, int width_lower, int width_range, int length_lower, int length_range, int map[MAP_X][MAP_Y], int area, int info[INFO_SIZE]);

void powerupGen(int type, int num, int map[MAP_X][MAP_Y]);

bool testMap(Actor & player, int map[MAP_X][MAP_Y], int win_x, int win_y, int win_length, int win_width);

bool validKeyPress(int key);

#endif
