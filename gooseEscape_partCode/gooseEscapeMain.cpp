//MAIN
//gooseEscapeMain.cpp

#include <BearLibTerminal.h>
#include <cmath>
#include <ctime>
#include <iostream>  
using namespace std;
#include "gooseEscapeUtil.hpp"
#include "gooseEscapeActor.hpp"
#include "gooseEscapeConsole.hpp"
#include "gooseEscapeGamePlay.hpp"

//set up the console. Don't modify this line!
Console out;

int main()
{
    // Set up for using rand()
    srand(time(NULL));

	//Set up the window
    terminal_open();
  	terminal_set(SETUP_MESSAGE);

	//Initialize Required variables
	int level_selected = 0; // what level is selected (Easy = 3, Medium = 4, Hard = 5)
	int goose_spawn_x = 0, goose_spawn_y = 0; // Goose spawn coordinates
	int player_spawn_x = 0, player_spawn_y = 0; //Player spawn coordinates
	bool win = 0;//used to check if player has touched win block
	int powerup = 0;//used to indicate active powerup
	int uses = 0;//used to indicate how many uses are avalible for active powerup
	int keyEntered = TK_A; //used to see what key was pressed
	
	/*
	used for randomizing spawn location
	distance_to_win_x measures the distance from the actor (player or goose)
	to the closest win location on the x axis
	distance_to_win_y does the same but on the y axis
	moves_to_win is the minimum number of moves required to reach the win location
	(p = player, g = goose)
	*/
	int player_distance_to_win_x = 0, player_distance_to_win_y = 0, p_moves_to_win = 0;
	int goose_distance_to_win_x = 0, goose_distance_to_win_y = 0, g_moves_to_win = 0;
	
	int map[MAP_X][MAP_Y] = {0}; //creates an 80x21 map of integers
	
	/* 
	win_info stores information regarding the win zone
	win_info[0] stores the x coordinate
	win_info[1] stores the y coordinate
	win_info[2] stores the length of the zone (size on x axis)
	win_info[3] stores the width of the zone (size on y axis)
	*/
	int win_info[WIN_INFO_SIZE] = {0};

	//Creates the difficultly selection zones																															
	for (int col = 0; col < LEVEL_SELECT_SIZE; col++)
  	{
  		for (int row = 0; row < LEVEL_SELECT_SIZE; row++)
  		{
	  		map[col + EASY_ZONE_X][row + DIFFICULTY_ZONE_Y] = EASY;
	      	map[col + MEDIUM_ZONE_X][row + DIFFICULTY_ZONE_Y] = MEDIUM;
	      	map[col + HARD_ZONE_X][row + DIFFICULTY_ZONE_Y] = HARD;
  		}
  	}

    // print the level selection map
	printBoard(map);


	//makes the player
	Actor player(PLAYER_CHAR, 40, 7, 100, PLAYER_COLOUR);
	
	
	//Level selection instructions
	out.writeLine("Use the arrow keys to move the player and select a difficulty!");
	out.writeLine("E - Easy");
	out.writeLine("M - Medium");
	out.writeLine("H - Hard");
	
	//movement required to select level
 	while(keyEntered != TK_ESCAPE && keyEntered != TK_CLOSE && level_selected == 0)
	{
	    keyEntered = terminal_read();

        if (keyEntered != TK_ESCAPE && keyEntered != TK_CLOSE)
    	    moveStarter(keyEntered, player, map, level_selected);	  
	}
	
	//after selecting the level clears the selection zones from the terminal and map
	terminal_clear_area(27, 9, 27, 3);
  	
  	for (int col = 0; col < LEVEL_SELECT_SIZE; col++)
  	{
  		for (int row = 0; row < LEVEL_SELECT_SIZE; row++)
  		{
	  		map[col + EASY_ZONE_X][row + DIFFICULTY_ZONE_Y] = EMPTY;
	      	map[col + MEDIUM_ZONE_X][row + DIFFICULTY_ZONE_Y] = EMPTY;
	      	map[col + HARD_ZONE_X][row + DIFFICULTY_ZONE_Y] = EMPTY;
  		}
  	}
  	
  	
	if (level_selected == 3) // easy map generation
  	{
  		generateRandomWall(1, 3, 1, 1, 3, 8, map);
  		generateRandomWin(2, 3, 2, 3, map, win_info);
  		
  		powerupGen(1,10,map);
  		powerupGen(2,5,map);
  	}
	else if (level_selected == 4) // medium map generation
  	{
  		generateRandomWall(3, 8, 1, 1, 5, 12, map);
  		generateRandomWin(2, 2, 2, 2, map, win_info);
  		
  		powerupGen(1,5,map);
  		powerupGen(2,2,map);
  		powerupGen(3,2,map);
    }
  
  	else // hard map generation
  	{
  		generateRandomWall(10, 11, 2, 3, 10, 11, map);
  		generateRandomWin(1, 1, 1, 1, map, win_info);

  		powerupGen(1,2,map);
  	}
  	
  	//randomizes player spawn point and makes sure to spawn in a clear area
  	do
  	{
  		player_spawn_x = rand() % 80;
  		player_spawn_y = rand() % 21;
  		player_distance_to_win_x = findClosestTile(player_spawn_x, win_info[0], win_info[2]) - player_spawn_x;
		player_distance_to_win_y = findClosestTile(player_spawn_y, win_info[1], win_info[3]) - player_spawn_y;
		p_moves_to_win = abs(player_distance_to_win_x) + abs(player_distance_to_win_y);
	} while(map[player_spawn_x][player_spawn_y] != 0 || 
			p_moves_to_win< level_selected*10);
	player.update_location(player_spawn_x - player.get_x(), player_spawn_y - player.get_y());
	
	generateWinPath(player, player_distance_to_win_x, player_distance_to_win_y, map);
	
  	//randomizes goose spawn point and makes sure player can win
  	do
  	{
  		goose_spawn_x = rand() % 80;
  		goose_spawn_y = rand() % 21;
  		goose_distance_to_win_x = findClosestTile(goose_spawn_x, win_info[0], win_info[2]) - goose_spawn_x;
		goose_distance_to_win_y = findClosestTile(goose_spawn_y, win_info[1], win_info[3]) - goose_spawn_y;
		g_moves_to_win = abs(goose_distance_to_win_x) + abs(goose_distance_to_win_y);
	} while(map[player_spawn_x][player_spawn_y] == 2 ||
			p_moves_to_win >= g_moves_to_win ||
			g_moves_to_win - p_moves_to_win < (6-level_selected)*4);
  	
	//make the goose
	Actor monster(MONSTER_CHAR, goose_spawn_x, goose_spawn_y, 100, GOOSE_COLOUR);
    
    printBoard(map);
    
	// Printing the game instructions in the console window
    out.writeLine("Escape the Goose! " + monster.get_location_string());
	out.writeLine("Use the arrow keys to move");
	out.writeLine("If the goose catches you, you lose!");
	out.writeLine("Be careful! Sometimes the goose can jump through walls!");
	//out.writeLine("Powerup 1: Move 2 squares for next 5 turns");
	//out.writeLine("Powerup 2: Move 10 squares for next turn");
	//out.writeLine("Powerup 3: Move through 1 wall anytime");
    
    //game movement
    while(keyEntered != TK_ESCAPE && keyEntered != TK_CLOSE 
                    && !captured(player,monster) && !win)
	{
	    // get player key press
	    keyEntered = terminal_read();

		//valid key press checks that the player is hitting one of the arrow keys or the escape or close key
		//otherwise if the player was hitting arbitrary keys the player would miss their turn and the goose would move
        if (keyEntered != TK_ESCAPE && keyEntered != TK_CLOSE && validKeyPress(keyEntered))
        {
        	//if no powerups are active, or powerup 3 is active (move through wall)
            if(powerup == 0  || powerup == 3)
            {
            	movePlayer(keyEntered, player, map, win, powerup, uses);
			}
            	
            //Powerup 1 - moves double squares for 5 turns
            else if(powerup == 1 && uses > 0)
            {
            	for(int moves = 0; moves < 2; moves++)
				{
					movePlayer(keyEntered, player, map, win, powerup, uses);
					powerup = 1;
					if(win)
					{
						moves = 2;
					}
				}
            	uses --;
            	
            	if (uses == 0)
					powerup = 0;
			}
			
			//Powerup 2 - moves 10 squares for 1 turn
			else if(powerup == 2)//jumping 10 squares
			{
				for(int moves = 0; moves < 10; moves++)
				{
					movePlayer(keyEntered, player, map, win, powerup, uses);
					if(win)
					{
						moves = 10;
					}
				}
            	powerup = 0;
			}
			
    	    if (!captured(player,monster) and !win) 
    	    {
    	    	moveGoose(player, monster, map); //moves the goose towards the player
			}
    	    
    	    
  		}
	}
	
    if (keyEntered != TK_CLOSE)
    {
      	//once we're out of the loop, the game is over
        if (win)
        	out.writeLine("You have won");
        	
        if (captured(player,monster))
 			out.writeLine("Goose has caught you");
	
        
        out.writeLine("Game has ended");
    
        // Output why:  did the goose get you?  Or did you win?
	
    	// Wait until user closes the window
        while (terminal_read() != TK_CLOSE);
    }
    
    

	//game is done, close it  
    terminal_close();
}
