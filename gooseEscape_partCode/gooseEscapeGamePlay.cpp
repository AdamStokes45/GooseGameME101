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

/*
    This file is all about the game world.  You will modify this to add
    functionality to your game, first to get it working, and later to make
    it fun.
    
    If you add or modify functions, be sure to update the prototypes in the
    gooseEscapeGamePlay.hpp file
*/

extern Console out;



void printBoard(int map[MAP_X][MAP_Y])
{
	for (int col = 0; col < MAP_X; col++)
  	{
  		for (int row = 0; row < MAP_Y; row++)
    	{
      		if (map[col][row] == 1)
      			terminal_put(col, row, WALL_CHAR);
            else if (map[col][row] == 2)
              terminal_put(col, row, WIN_CHAR);
            else if (map[col][row] == 3)
              terminal_put(col, row, EASY_CHAR);	
            else if (map[col][row] == 4)
              terminal_put(col, row, MEDIUM_CHAR);
            else if (map[col][row] == 5)
              terminal_put(col, row, HARD_CHAR);
            else if (map[col][row] == 6)
              terminal_put(col, row, POWER_1);
            else if (map[col][row] == 7)
              terminal_put(col, row, POWER_2);
            else if (map[col][row] == 8)
              terminal_put(col, row, POWER_3);
    	}
  	}
	// after putting items on the game board, refresh the terminal to see the items
	terminal_refresh();
}

void setArea(int length, int width, int area, int map[MAP_X][MAP_Y], int info[INFO_SIZE])
{
	int spawn_x = rand() % (MAP_X - length);
    int spawn_y = rand() % (MAP_Y - length);
	for (int col = 0; col < length; col++)
    {
       	for (int row = 0; row < width; row++)
       	{
       		map[spawn_x + col][spawn_y + row] = area;
		}
	}
	info[0] = spawn_x;
	info[1] = spawn_y;
}
void generateRandomArea(int amount_lower, int amount_range, int width_lower, int width_range, int length_lower, int length_range, int map[MAP_X][MAP_Y], int area, int info[INFO_SIZE])
{
	int amount = rand() % amount_range + amount_lower;
	for (int num = 0; num < amount; num++)
	{
		int width = rand() % width_range + width_lower;
    	int length = rand() % length_range + length_lower;
		info[2] = length;
		info[3] = width;
      	int orientation = rand() % 2;
	      
      	if (orientation == 0)
			setArea(length, width, area, map, info);
      	else
      		setArea(width, length, area, map, info);

	}
}


/*
    Do something when the goose captures the player

    At the moment the function just checks to see if the player and the goose
    are in the same location.  If you want to attack or do something else, this
    function would need to change.  For example, maybe the two touch each other
    and then fight.  You could use the health that is given in the Actor class,
    and update it.  Fight, run, use weapons, it is up to you!
*/

bool captured(Actor const & player, Actor const & monster)
{
    return (player.get_x() == monster.get_x() 
         && player.get_y() == monster.get_y());
}

/*
    Move the player to a new location based on the user input
    
    All key presses start with "TK_" then the character.  So "TK_A" is the A
    key that was pressed.  At the moment, only the arrow keys are used,
    but feel free to add more keys that allow the player to do something else
    like pick up a power up.
    
    A look-up table might be useful.

    Going further: You could decide to learn about switch statements
*/
void movePlayer(int key, Actor & player, int map[MAP_X][MAP_Y],bool & win, int & powerup, int & uses)
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
	
	
	if (player.can_move(xMove, yMove) &&
		map[player.get_x() + xMove][player.get_y() + yMove] != WALL)
		{
			player.update_location(xMove, yMove);
		}
	else if(player.can_move(xMove, yMove) &&
		map[player.get_x() + xMove][player.get_y() + yMove] == WALL && powerup == 3)//only works for walls up to 4 for some reason, and walls bordered to walls glitch it out
		{
			int wall_hop = 1;
			while(map[player.get_x() + xMove*wall_hop][player.get_y() + yMove*wall_hop] == WALL)
			{
				wall_hop += 1;
			}
			player.update_location(wall_hop*xMove, wall_hop*yMove);
			powerup = 0;
			uses = 0;
		}
		
	int location_on_map = map[player.get_x()][player.get_y()];//useless as of now
	
	if(map[player.get_x()][player.get_y()] > 0)//only runs if hits a powerup or win square
	{
		if (map[player.get_x()][player.get_y()] == 2)//win square num
			win = 1;
			
		if (map[player.get_x()][player.get_y()] == 6)//first powerup square num
		{
			powerup = 1;
			uses = 5;//clicks the powerup is good for
			map[player.get_x()][player.get_y()] = 0;//resets the tile so doesnt reprint when map is re-printed
		}
		
		if (map[player.get_x()][player.get_y()] == 7)//second powerup square num
		{
			powerup = 2;
			map[player.get_x()][player.get_y()] = 0;
		}    
		 
		if (map[player.get_x()][player.get_y()] == 8)//second powerup square num
		{
			powerup = 3;
			map[player.get_x()][player.get_y()] = 0;
		}  
		
			
		
	}
	
		
	
}

void moveGoose(Actor & player, Actor & goose,  int map[MAP_X][MAP_Y])
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
    
    if (map[goose.get_x()][goose.get_y()] != 0)
    {
    	goose.update_location(xMove, yMove);//moves goose
		printBoard(map);//re prints board where to cover where goose overlapped
   		goose.update_location(0,0);//reprints goose incase hes still on the wall and got written over
	}
	else
	{
		goose.update_location(xMove, yMove);//if hes not on a wall, he just moves
	}
  
}
void moveStarter(int key, Actor & starter, int map[MAP_X][MAP_Y], int & level_selected)
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
			starter.update_location(xMove, yMove);
    
    if (map[starter.get_x()][starter.get_y()] != 0)
		level_selected = map[starter.get_x()][starter.get_y()];
}

void powerupGen(int type, int num, int map[MAP_X][MAP_Y])
{
	for(int count = 0; count < num; count++)
	{
		int x_power = rand() % 80;
		int y_power = rand() % 21;
		
		bool placed = false;
		
		while(!placed)
		{
			if(map[x_power][y_power] == 0)
			{
				map[x_power][y_power] = type + 5;// + 5 because it should be 6 char for the first powerup
				placed = true;
			}
		}	
		
	}
	
	map[40][1] = 6;
	map[42][1] = 7;
}

bool validKeyPress(int key)
{
	int validKeys[6] = {TK_UP, TK_RIGHT, TK_LEFT, TK_DOWN, TK_ESCAPE, TK_CLOSE};
	for (int i = 0; i < 6; i++)
	{
		if (key == validKeys[i])
			return true;
	}
	return false;
}

int findClosestTile(int player_location, int win_location, int win_size)
{
	int closestTile = 0;
	int distance = 1000;
	for (int tileNum = 0; tileNum < win_size; tileNum++)
	{
		if (abs(win_location+tileNum-player_location) < distance)
		{
			distance = abs(win_location-player_location);
			closestTile = win_location + tileNum;
		}
	}
	return closestTile;
}

bool testMap(Actor & player, int map[MAP_X][MAP_Y], int win_x, int win_y, int win_length, int win_width, bool & win)
{
	int count = 0;
	int closestWinX = findClosestTile(player.get_x(), win_x, win_length);
	int closestWinY = findClosestTile(player.get_y(), win_y, win_width);
	int yMove = 0, xMove = 0;
    if (abs(player.get_x() - closestWinX) > abs(player.get_y() - closestWinY) && map[player.get_x() + 1][player.get_y()] != WALL && map[player.get_x() - 1][player.get_y()] != WALL)
    {
    	if (player.get_x() < closestWinX)
      		xMove = 1;
		else 
      		xMove = -1;
	}
    else
    {
      	if (player.get_y() < closestWinY && map[player.get_x()][player.get_y()+1] != WALL)
        	yMove = 1;
    	else if (map[player.get_x()][player.get_y()-1] != WALL)
        	yMove = -1;
    }
    	if (player.can_move(xMove, yMove) &&
		map[player.get_x() + xMove][player.get_y() + yMove] != WALL)
			player.update_virtual_location(xMove, yMove);
			
		if (map[player.get_x()][player.get_y()] == 2)//win square num
		win = 1;
}



