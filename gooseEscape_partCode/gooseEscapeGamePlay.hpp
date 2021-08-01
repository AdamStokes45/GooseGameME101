//GAMEPLAY HEADER
//gooseEscapeGamePlay.hpp

#ifndef GOOSE_ESCAPE_GAMEPLAY
#define GOOSE_ESCAPE_GAMEPLAY
#include "gooseEscapeUtil.hpp"
#include "gooseEscapeActor.hpp"
#include "gooseEscapeConsole.hpp"


const int EMPTY = 0;
const int WALL = 1;
const int WINNER = 2;
const int EASY = 3;
const int MEDIUM = 4;
const int HARD = 5;
const int POWER_1 = 6;
const int POWER_2 = 7;
const int POWER_3 = 8;

const int MAP_X = 80;
const int MAP_Y = 21;
const int LEVEL_SELECT_SIZE = 3;
const int INFO_SIZE = 4;

//display characters
const int PLAYER_CHAR = int('@');
const int MONSTER_CHAR = int('G');
const int WALL_CHAR = int('o');
const int WIN_CHAR = int('%'); //% sign, a special character used in the ancient game "Rogue"
const int EASY_CHAR = int('E');
const int MEDIUM_CHAR = int('M');
const int HARD_CHAR = int('H');
const int POWER_1_CHAR = int('1');
const int POWER_2_CHAR = int('2');
const int POWER_3_CHAR = int('3');


const char PLAYER_COLOUR[] = "light sky";
const char GOOSE_COLOUR[] = "yellow";


bool captured(Actor const & player, Actor const & monster);

void movePlayer(int key, Actor & player, int map[MAP_X][MAP_Y],bool & win, int & powerup, int & uses);

void printBoard(int map[MAP_X][MAP_Y]);

void moveGoose(Actor & monster, Actor & player, int map[MAP_X][MAP_Y]);

void moveStarter(int key, Actor & starter, int map[MAP_X][MAP_Y], int & level_selected);

void generateRandomWin(int width_lower, int width_range, int length_lower, int length_range, int map[MAP_X][MAP_Y], int info[INFO_SIZE]);

void generateRandomWall(int amount_lower, int amount_range, int width_lower, int width_range, int length_lower, int length_range, int map[MAP_X][MAP_Y]);

void powerupGen(int type, int num, int map[MAP_X][MAP_Y]);

bool testMap(Actor & player, int map[MAP_X][MAP_Y], int win_x, int win_y, int win_length, int win_width, bool & win);

bool validKeyPress(int key);

#endif
