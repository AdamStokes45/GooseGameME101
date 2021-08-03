//GAMEPLAY HEADER
//gooseEscapeGamePlay.hpp

#ifndef GOOSE_ESCAPE_GAMEPLAY
#define GOOSE_ESCAPE_GAMEPLAY
#include "gooseEscapeUtil.hpp"
#include "gooseEscapeActor.hpp"
#include "gooseEscapeConsole.hpp"

//values for different characters on the map
const int EMPTY = 0;
const int WALL = 1;
const int WINNER = 2;
const int EASY = 3;
const int MEDIUM = 4;
const int HARD = 5;
const int POWER_1 = 6;
const int POWER_2 = 7;
const int POWER_3 = 8;

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

//map constants
const int MAP_X = 80;
const int MAP_Y = 21;
const int LEVEL_SELECT_SIZE = 3;

//Difficulty selection zones coordinates
const int EASY_ZONE_X = 27;
const int MEDIUM_ZONE_X = 39;
const int HARD_ZONE_X = 51;
const int DIFFICULTY_ZONE_Y = 9;

//Easy Difficulty Constants
const int E_WALL_AMOUNT_LOWER = 1; //Wall constants
const int E_WALL_AMOUNT_RANGE = 3;
const int E_WALL_WIDTH_LOWER = 1;
const int E_WALL_WIDTH_RANGE = 1;
const int E_WALL_LENGTH_LOWER = 3;
const int E_WALL_LENGTH_RANGE = 8;
const int E_WIN_WIDTH_LOWER = 2; //Win zone constants
const int E_WIN_WIDTH_RANGE = 3;
const int E_WIN_LENGTH_LOWER = 2;
const int E_WIN_LENGTH_RANGE = 3;
const int E_POW1_NUM = 10; //Powerup constants
const int E_POW2_NUM = 5;


//Medium Difficulty Constants
const int M_WALL_AMOUNT_LOWER = 3; //Wall constants
const int M_WALL_AMOUNT_RANGE = 8;
const int M_WALL_WIDTH_LOWER = 1;
const int M_WALL_WIDTH_RANGE = 1;
const int M_WALL_LENGTH_LOWER = 5;
const int M_WALL_LENGTH_RANGE = 12;
const int M_WIN_WIDTH_LOWER = 2; //Win zone constants
const int M_WIN_WIDTH_RANGE = 2;
const int M_WIN_LENGTH_LOWER = 2;
const int M_WIN_LENGTH_RANGE = 2;
const int M_POW1_NUM = 5; //Powerup constants
const int M_POW2_NUM = 2;
const int M_POW3_NUM = 2;

//Hard Difficulty Constants
const int H_WALL_AMOUNT_LOWER = 10; //Wall constants
const int H_WALL_AMOUNT_RANGE = 11;
const int H_WALL_WIDTH_LOWER = 2;
const int H_WALL_WIDTH_RANGE = 3;
const int H_WALL_LENGTH_LOWER = 10;
const int H_WALL_LENGTH_RANGE = 11;
const int H_WIN_WIDTH_LOWER = 1; //Win zone constants
const int H_WIN_WIDTH_RANGE = 1;
const int H_WIN_LENGTH_LOWER = 1;
const int H_WIN_LENGTH_RANGE = 1;
const int H_POW1_NUM = 2; //Powerup constants

//win info constants
const int WIN_INFO_SIZE = 4;
const int WIN_X_INDEX = 0;
const int WIN_Y_INDEX = 1;
const int WIN_LEN_INDEX = 2;
const int WIN_WIDTH_INDEX = 3;

//actor constants
const char PLAYER_COLOUR[] = "light sky";
const char GOOSE_COLOUR[] = "yellow";

//other constants
const int POW1_USES = 5;
const int VALID_KEY_AMOUNT = 6;

bool captured(Actor const & player, Actor const & monster);

void movePlayer(int key, Actor & player, int map[MAP_X][MAP_Y],bool & win, int & powerup, int & uses);

void printBoard(int map[MAP_X][MAP_Y]);

void moveGoose(Actor & monster, Actor & player, int map[MAP_X][MAP_Y]);

void moveStarter(int key, Actor & starter, int map[MAP_X][MAP_Y], int & level_selected);

void generateRandomWin(int width_lower, int width_range, int length_lower, int length_range, int map[MAP_X][MAP_Y], int info[WIN_INFO_SIZE]);

void generateRandomWall(int amount_lower, int amount_range, int width_lower, int width_range, int length_lower, int length_range, int map[MAP_X][MAP_Y]);

void powerupGen(int type, int num, int map[MAP_X][MAP_Y]);

int findClosestTile(int player_location, int win_location, int win_size);

void generateWinPath(Actor & player, int distance_x, int distance_y, int map[MAP_X][MAP_Y]);

bool validKeyPress(int key);

#endif
