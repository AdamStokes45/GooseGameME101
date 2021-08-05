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
	int goose2_spawn_x = 0, goose2_spawn_y = 0; // Goose2 spawn coordinates
	int player_spawn_x = 0, player_spawn_y = 0; //Player spawn coordinates
	bool win = 0;//used to check if player has touched win block
	int powerup = 0;//used to indicate active powerup
	int uses = 0;//used to indicate how many uses are avalible for active powerup
	int keyEntered = TK_A; //used to see what key was pressed
	
	/*
	used for randomizing spawn location
	distance_to_win_x measures the distance from the actor 
	to the closest win location on the x axis
	distance_to_win_y does the same but on the y axis
	moves_to_win is the minimum number of moves required to reach the win location
	(p = player)
	*/
	int player_distance_to_win_x = 0, player_distance_to_win_y = 0, p_moves_to_win = 0;
	
	int map[ROW_SIZE][COL_SIZE] = {0}; //creates an 80x21 map of integers
	
	/* 
	win_info stores information regarding the win zone
	win_info[0] stores the x coordinate
	win_info[1] stores the y coordinate
	win_info[2] stores the length of the zone (size on x axis)
	win_info[3] stores the width of the zone (size on y axis)
	*/
	int win_info[WIN_INFO_SIZE] = {0};
	
	//Creates the difficultly selection zones																															
	for (int row = 0; row < LEVEL_SELECT_SIZE; row++)
  	{
  		for (int col = 0; col < LEVEL_SELECT_SIZE; col++)
  		{
	  		map[row + DIFFICULTY_ZONE_Y][col + EASY_ZONE_X] = EASY;
	      	map[row + DIFFICULTY_ZONE_Y][col + MEDIUM_ZONE_X] = MEDIUM;
	      	map[row + DIFFICULTY_ZONE_Y][col + HARD_ZONE_X] = HARD;
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
	terminal_clear_area(EASY_ZONE_X, DIFFICULTY_ZONE_Y, LEVEL_SELECT_SIZE, LEVEL_SELECT_SIZE);
	terminal_clear_area(MEDIUM_ZONE_X, DIFFICULTY_ZONE_Y, LEVEL_SELECT_SIZE, LEVEL_SELECT_SIZE);
	terminal_clear_area(HARD_ZONE_X, DIFFICULTY_ZONE_Y, LEVEL_SELECT_SIZE, LEVEL_SELECT_SIZE);
  	
  	for (int row = 0; row < LEVEL_SELECT_SIZE; row++)
  	{
  		for (int col = 0; col < LEVEL_SELECT_SIZE; col++)
  		{
	  		map[row + DIFFICULTY_ZONE_Y][col + EASY_ZONE_X] = EMPTY;
	      	map[row + DIFFICULTY_ZONE_Y][col + MEDIUM_ZONE_X] = EMPTY;
	      	map[row + DIFFICULTY_ZONE_Y][col + HARD_ZONE_X] = EMPTY;
  		}
  	}
  	
  	
	if (level_selected == EASY) // easy map generation
  	{
  		generateRandomWall(E_WALL_AMOUNT_LOWER, E_WALL_AMOUNT_RANGE, 
		  				   E_WALL_WIDTH_LOWER, E_WALL_WIDTH_RANGE, 
						   E_WALL_LENGTH_LOWER, E_WALL_LENGTH_RANGE, map);
  		generateRandomWin(E_WIN_WIDTH_LOWER, E_WIN_WIDTH_RANGE, 
		  				  E_WIN_LENGTH_LOWER, E_WIN_LENGTH_RANGE, map, win_info);
  		
  		powerupGen(POWER_1, E_POW1_NUM, map);
  		powerupGen(POWER_2, E_POW2_NUM, map);
  	}
	else if (level_selected == MEDIUM) // medium map generation
  	{
		generateRandomWall(M_WALL_AMOUNT_LOWER, M_WALL_AMOUNT_RANGE, 
		  				   M_WALL_WIDTH_LOWER, M_WALL_WIDTH_RANGE, 
						   M_WALL_LENGTH_LOWER, M_WALL_LENGTH_RANGE, map);
		
  		generateRandomWin(M_WIN_WIDTH_LOWER, M_WIN_WIDTH_RANGE, 
		  				  M_WIN_LENGTH_LOWER, M_WIN_LENGTH_RANGE, map, win_info);
  		
  		powerupGen(POWER_1, M_POW1_NUM, map);
  		powerupGen(POWER_2, M_POW2_NUM, map);
  		powerupGen(POWER_3, M_POW3_NUM, map);
  		
    }
  
  	else // hard map generation
  	{
		generateRandomWall(H_WALL_AMOUNT_LOWER, H_WALL_AMOUNT_RANGE, 
		  				   H_WALL_WIDTH_LOWER, H_WALL_WIDTH_RANGE, 
						   H_WALL_LENGTH_LOWER, H_WALL_LENGTH_RANGE, map);
  		generateRandomWin(H_WIN_WIDTH_LOWER, H_WIN_WIDTH_RANGE, 
		  				  H_WIN_LENGTH_LOWER, H_WIN_LENGTH_RANGE, map, win_info);

  		powerupGen(POWER_3, H_POW3_NUM, map);
  	}
  	//randomizes player spawn point and makes sure to spawn in a clear area
  	do
  	{
  		player_spawn_x = rand() % COL_SIZE;
  		player_spawn_y = rand() % ROW_SIZE;
  		player_distance_to_win_x = findClosestTile(player_spawn_x, win_info[WIN_X_INDEX], 
		  										   win_info[WIN_LEN_INDEX]) - player_spawn_x;
		player_distance_to_win_y = findClosestTile(player_spawn_y, win_info[WIN_Y_INDEX], 
												   win_info[WIN_WIDTH_INDEX]) - player_spawn_y;
		p_moves_to_win = abs(player_distance_to_win_x) + abs(player_distance_to_win_y);
	} while(map[player_spawn_y][player_spawn_x] != 0 || 
			p_moves_to_win < level_selected * 10);
	player.update_location(player_spawn_x - player.get_x(), player_spawn_y - player.get_y(), true);
	generateWinPath(player, player_distance_to_win_x, player_distance_to_win_y, map);
	
  	//randomizes goose spawn point and makes sure player can win
	randomGooseSpawn(goose_spawn_x, goose_spawn_y, player_spawn_x, player_spawn_y, p_moves_to_win, level_selected, win_info, map);
		
	//make the goose
	Actor monster(MONSTER_CHAR, goose_spawn_x, goose_spawn_y, 100, GOOSE_COLOUR);

	//If the difficulty is hard, spawn another goose
	if (level_selected == HARD)
	{
		do
		{
			randomGooseSpawn(goose2_spawn_x, goose2_spawn_y, player_spawn_x, player_spawn_y, p_moves_to_win, level_selected, win_info, map);
		} while (goose2_spawn_x == goose_spawn_x and goose2_spawn_y == goose_spawn_y);
		
	}
	Actor monster2(MONSTER_CHAR, goose2_spawn_x, goose2_spawn_y, 100, GOOSE_COLOUR);
	if (level_selected == EASY or level_selected == MEDIUM)
		terminal_clear_area(goose2_spawn_x, goose2_spawn_y, 1, 1);
		
    printBoard(map);
    
	// Printing the game instructions in the console window
    out.writeLine("Escape the Goose! Don't let the goose catch you! The goose can fly over walls! ");
	out.writeLine("Powerup 1: Move 2 squares for next 5 turns");
	out.writeLine("Powerup 2: Move 10 squares for next turn");
	out.writeLine("Powerup 3: Move through 1 wall anytime");
    
    //game movement
    while(keyEntered != TK_ESCAPE && keyEntered != TK_CLOSE 
                    && !captured(player,monster) && !win && !captured(player, monster2))
	{
	    //get player key press
	    keyEntered = terminal_read();

		/* valid key press checks that the player is hitting one of the arrow keys 
		or the escape or close keyotherwise if the player was hitting arbitrary keys 
		the player would miss their turn and the goose would move*/
        if (keyEntered != TK_ESCAPE && keyEntered != TK_CLOSE && validKeyPress(keyEntered))
        {
        	//if no powerups are active, or powerup 3 is active (move through wall)
            if(powerup == 0  || powerup == POWER_3)
            	movePlayer(keyEntered, player, map, win, powerup, uses);
            	
            //Powerup 1 -> moves double squares for 5 turns
            else if(powerup == POWER_1 && uses > 0)
            {
            	for(int moves = 0; moves < 2; moves++)
				{
					movePlayer(keyEntered, player, map, win, powerup, uses);
					powerup = POWER_1;
					if(win)//If you reach the win location you stop
						moves = 2;
				}
            	uses --;
            	
            	if (uses == 0)//resets powerup
					powerup = 0;
			}
			
			//Powerup 2 -> moves 10 squares for 1 turn
			else if(powerup == POWER_2)
			{
				for(int moves = 0; moves < 10; moves++)
				{
					movePlayer(keyEntered, player, map, win, powerup, uses);
					if(win) //If you reach the win location you stop
						moves = 10;
				}
            	powerup = 0;//resets powerup
			}
			
			//if the player hasn't been caught and hasn't won
    	    if (!captured(player,monster) and !win and !captured(player, monster2)) 
    	    {
    	    	moveGoose(player, monster, map); //moves the goose towards the player
    	    	if(level_selected == 5)//moving second goose
    	    		moveGoose(player, monster2, map);
    	    		monster.update_location(0,0,true);
			}
    	    	
    	    
    	    
  		}
	}
	
    if (keyEntered != TK_CLOSE)
    {
      	//once we're out of the loop, the game is over
        if (win)
        	out.writeLine("You have won");
        else if (captured(player, monster) or captured(player, monster2))
 			out.writeLine("Goose has caught you");
	
        out.writeLine("Game has ended");
	
    	// Wait until user closes the window
        while (terminal_read() != TK_CLOSE);
    }
    
    

	//game is done, close it  
    terminal_close();
}
