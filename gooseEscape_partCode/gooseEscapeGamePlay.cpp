//GAMEPLAY C++
//gooseEscapeGamePlay.cpp

#include <iostream>
#include <cmath>
using namespace std;
#include <BearLibTerminal.h>
#include "gooseEscapeUtil.hpp"
#include "gooseEscapeActor.hpp"
#include "gooseEscapeConsole.hpp"
#include "gooseEscapeGamePlay.hpp"

//required
extern Console out;


//print the game board with specified values for characters
//print the game board with specified values for characters
void printBoard(int map[ROW_SIZE][COL_SIZE])
{
	for (int row = 0; row < ROW_SIZE; row++)
  	{
  		for (int col = 0; col < COL_SIZE; col++)
    	{
      		if (map[row][col] == 1)
      			terminal_put(col, row, WALL_CHAR);
      			
            else if (map[row][col] == 2)
              terminal_put(col, row, WIN_CHAR);
              
            else if (map[row][col] == EASY)
              terminal_put(col, row, EASY_CHAR);	
              
            else if (map[row][col] == MEDIUM)
              terminal_put(col, row, MEDIUM_CHAR);
              
            else if (map[row][col] == HARD)
              terminal_put(col, row, HARD_CHAR);
              
            else if (map[row][col] == POWER_1)
              terminal_put(col, row, POWER_1_CHAR);
              
            else if (map[row][col] == POWER_2)
              terminal_put(col, row, POWER_2_CHAR);
              
            else if (map[row][col] == POWER_3)
              terminal_put(col, row, POWER_3_CHAR);
              
    	}
  	}
	// after putting items on the game board, refresh the terminal to see the items
	terminal_refresh();
}

//places winning squares down onto board
void setWin(int length, int width, int map[ROW_SIZE][COL_SIZE], int win_info[WIN_INFO_SIZE])
{
	int spawn_x = rand() % (COL_SIZE - length);
    int spawn_y = rand() % (ROW_SIZE - length);
	for (int row = 0; row < width; row++)
    {
       	for (int col = 0; col < length; col++)
       	{
       		map[spawn_y + row][spawn_x + col] = WINNER;
		}
	}
	win_info[WIN_X_INDEX] = spawn_x;
	win_info[WIN_Y_INDEX] = spawn_y;
}

//generates random win location
void generateRandomWin(int width_lower, int width_range, int length_lower, int length_range, int map[ROW_SIZE][COL_SIZE], int win_info[WIN_INFO_SIZE])
{

	int width = rand() % width_range + width_lower;
    int length = rand() % length_range + length_lower;
	win_info[WIN_LEN_INDEX] = length;
	win_info[WIN_WIDTH_INDEX] = width;
    int orientation = rand() % 2;
	 
    if (orientation == 0)
		setWin(length, width, map, win_info);
    else
      	setWin(width, length, map, win_info);


}

void setWall (int length, int width, int map[ROW_SIZE][COL_SIZE])
{
	int spawn_x = rand() % (COL_SIZE - length);
    int spawn_y = rand() % (ROW_SIZE - length);
	for (int row = 0; row < length; row++)
    {
       	for (int col = 0; col < width; col++)
       	{
       		map[spawn_y + row][spawn_x + col] = WALL;
		}
	}
}

void generateRandomWall(int amount_lower, int amount_range, int width_lower, int width_range, int length_lower, int length_range, int map[ROW_SIZE][COL_SIZE])
{
	int amount = rand() % amount_range + amount_lower;
	for (int num = 0; num < amount; num++)
	{
		int width = rand() % width_range + width_lower;
    	int length = rand() % length_range + length_lower;
      	int orientation = rand() % 2;
      	if (orientation == 0)
			setWall(length, width, map);
      	else
      		setWall(width, length, map);

	}
}

//checks if goose has caught player
bool captured(Actor const & player, Actor const & monster)
{
    return (player.get_x() == monster.get_x() 
         && player.get_y() == monster.get_y());
}


//function that moves player, while checking for powerup
void movePlayer(int key, Actor & player, int map[ROW_SIZE][COL_SIZE],bool & win, int & powerup, int & uses)
{
    int yMove = 0, xMove = 0;
    if (key == TK_UP)
        yMove = -1;
    else if (key == TK_DOWN)
        yMove = 1;
    else if (key == TK_LEFT)
        xMove = -1;
    else if (key == TK_RIGHT)
        xMove = 1;
	
	//moves player regularly
	if (player.can_move(xMove, yMove) &&
		map[player.get_y() + yMove][player.get_x() + xMove] != WALL)
		{
			player.update_location(xMove, yMove,true);
		}
	//case when powerup 3 (move through wall) is active, and wall is infront of player
	else if(player.can_move(xMove, yMove) &&
		map[player.get_y() + yMove][player.get_x() + xMove] == WALL && powerup == POWER_3)
		{
			int wall_hop = 1;
			while(map[player.get_y() + yMove*wall_hop][player.get_x() + xMove*wall_hop] == WALL)
			{
				wall_hop += 1;
			}
			player.update_location(wall_hop*xMove, wall_hop*yMove,true);
			powerup = 0;
			uses = 0;
		}
		
	int location_on_map = map[player.get_y()][player.get_x()];//useless as of now
	
	//if the player has gone on a powerup, or win
	if(map[player.get_y()][player.get_x()] > 0)//only runs if hits a powerup or win square
	{
		if (map[player.get_y()][player.get_x()] == WINNER)//win square num
			win = 1;
			
		if (map[player.get_y()][player.get_x()] == POWER_1)//first powerup square num
		{
			powerup = POWER_1;
			uses = POW1_USES; //turns the powerup is good for
			map[player.get_y()][player.get_x()] = 0;//resets the tile so doesnt reprint when map is re-printed
		}
		
		if (map[player.get_y()][player.get_x()] == POWER_2)//second powerup square num
		{
			powerup = POWER_2;
			map[player.get_y()][player.get_x()] = 0;
		}    
		 
		if (map[player.get_y()][player.get_x()] == POWER_3)//second powerup square num
		{
			powerup = POWER_3;
			map[player.get_y()][player.get_x()] = 0;
		}  	
	}		
}


//function that moves the goose
void moveGoose(Actor & player, Actor & goose, int map[ROW_SIZE][COL_SIZE])
{
	int yMove = 0, xMove = 0;
    if (abs(player.get_x() - goose.get_x()) > abs(player.get_y() - goose.get_y()))
    {
    	if (player.get_x() > goose.get_x())
      		xMove = 1;
		else
      		xMove = -1;
	}
    else
    {
      if (player.get_y() > goose.get_y())
          yMove = 1;
        else
          yMove = -1;
    }
    
    //used to re-print the gameboard if the goose flys over a wall
    if (map[goose.get_y()][goose.get_x()] != 0)
    {
    	goose.update_location(xMove, yMove,true);
		printBoard(map);
   		goose.update_location(0,0,true);
	}
	else
	{
		goose.update_location(xMove, yMove,true);//if hes not on a wall, he just moves
	}
  
}

//starter is the object used in the level selection stage, same as movePlayer
void moveStarter(int key, Actor & starter, int map[ROW_SIZE][COL_SIZE], int & level_selected)
{
	int yMove = 0, xMove = 0;
    if (key == TK_UP)
        yMove = -1;
    else if (key == TK_DOWN)
        yMove = 1;
    else if (key == TK_LEFT)
        xMove = -1;
    else if (key == TK_RIGHT)
        xMove = 1;
    
    if (starter.can_move(xMove, yMove)) 
			starter.update_location(xMove, yMove,true);
    
    if (map[starter.get_y()][starter.get_x()] != 0)
		level_selected = map[starter.get_y()][starter.get_x()];
}

//places specified powerups onto the map
void powerupGen(int type, int num, int map[ROW_SIZE][COL_SIZE])
{
	int x_power = 0;
	int y_power = 0;
	bool placed = false;
	for(int count = 0; count < num; count++)
	{
		placed = false;
		
		while(!placed)
		{
			y_power = rand() % ROW_SIZE;
			x_power = rand() % COL_SIZE;
			if(map[y_power][x_power] == EMPTY)
			{
				map[y_power][x_power] = type;
				placed = true;
			}
		}	
		
	}
}

//checks if the key entered is valid
bool validKeyPress(int key)
{
	int validKeys[VALID_KEY_AMOUNT] = {TK_UP, TK_RIGHT, TK_LEFT, TK_DOWN, TK_ESCAPE, TK_CLOSE};
	for (int i = 0; i < VALID_KEY_AMOUNT; i++)
	{
		if (key == validKeys[i])
			return true;
	}
	return false;
}

//finds the closest win tile on one axis
int findClosestTile(int player_location, int win_location, int win_size)
{
	int closestTile = 0;
	int distance = 1000; //distance starts as an arbitrary large number 
	for (int tileNum = 0; tileNum < win_size; tileNum++)
	{
		if (abs(win_location + tileNum-player_location) < distance)
		{
			distance = abs(win_location-player_location);
			closestTile = win_location + tileNum;
		}
	}
	return closestTile;
}

/*this function randomizes the win path length in one dimension
The boolean x variable determines which orienation the path is.
If x is true that means it's a path on the x axis. If it is false it is a 
path on the y axis.
For a path on the x axis, the position parameter is it's x coordinate (which will change),
and position2 is it's y coordinate (which will stay constant). For a path on 
the y axis it's fliped (position -> y, position2 -> x)
The distance parameter is the distance to the win zone on that axis*/
void randomPath(int & position, int position2, int & distance, int map[ROW_SIZE][COL_SIZE], bool x)
{
	if (distance == 0)
		return;
	/*this variable determines whether or not the player will move in a positive 
	direction. If it is 1 they move in the positive direction, if it is -1
	they move in the negative direction */
	int positive = 1; 
	//This picks a random path length from 1 to the distance to the win zone
	int path_length = rand() % abs(distance) + 1;
	if (distance < 0) 
		positive = -1;
	//for the randomized path length, make those coordinates on the map empty (removes walls in the way)
	for(int pos = 1; pos <= path_length; pos++)
	{
		//if the path is x oriented, make the path on the x axis
		if (x) 
			map[position2][position + pos * positive] = EMPTY;
		else //else make it on the y axis
			map[position + pos * positive][position2] = EMPTY;
	}
	//updates current position and distance to win zone 
	position += path_length*positive;
	distance -= path_length*positive;

}

//this function generates an empty path to the win zone to ensure each level is winnable
void generateWinPath(Actor & player, int distance_x, int distance_y, int map[ROW_SIZE][COL_SIZE])
{
	int player_x = player.get_x(), player_y = player.get_y();
	//this while loop continues untill it is beside the win zone
	while(distance_x > 1 or distance_y > 1)
	{
		//triggers random path function for the x axis path
		randomPath(player_x, player_y, distance_x, map, true);
		//triggers random path function for the y axis path
		randomPath(player_y, player_x, distance_y, map, false);
	}
}
//generates random spawn location for a goose
void randomGooseSpawn(int & goose_spawn_x, int & goose_spawn_y, int player_spawn_x, 
					  int player_spawn_y, int p_moves_to_win, int level_selected, 
					  int win_info[WIN_INFO_SIZE], int map[ROW_SIZE][COL_SIZE])
{
	int g_moves_to_win = 0;
	/*do while loop continues looping if any of the three following conditions are true:
	-goose spawns closer to win zone than the player
	-goose doesn't spawn on an empty area
	-the difference between the moves to win is less than a certain amount 
	(this amount is determined by the level selected, Easy -> 6, Medium -> 4, Hard -> 2)*/
	do
  	{
  		goose_spawn_x = rand() % COL_SIZE;
  		goose_spawn_y = rand() % ROW_SIZE;
		g_moves_to_win = abs(findClosestTile(goose_spawn_x, win_info[WIN_X_INDEX], 
		  					win_info[WIN_LEN_INDEX]) - goose_spawn_x) + 
							abs(findClosestTile(goose_spawn_y, win_info[WIN_Y_INDEX], 
							win_info[WIN_WIDTH_INDEX]) - goose_spawn_y);
	} while(p_moves_to_win >= g_moves_to_win ||
			g_moves_to_win - p_moves_to_win < (6-level_selected)*2 ||
			map[goose_spawn_y][goose_spawn_x] != EMPTY);
	/*(6-level_selected)*2 is the math needed to calculate the minimum 
	difference between the moves to win of the player and goose */
}
