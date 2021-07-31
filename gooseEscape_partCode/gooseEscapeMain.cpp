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
    // Set up for using rand() to generate random numbers (can be deleted if random numbers not used)
    srand(time(NULL));

	//Set up the window.  Don't edit these two lines
    terminal_open();
  	terminal_set(SETUP_MESSAGE);

	//Initialize Required variables
	int level_selected = 0;
	int goose_spawn_x = 0, goose_spawn_y = 0, player_spawn_x = -1, player_spawn_y = -1;
	int win_info[INFO_SIZE] = {0};
	bool win = 0;
	int powerup = 0;
	int uses = 0;
	int keyEntered = TK_A;
	
	int map[MAP_X][MAP_Y] = {0};


	//easy medium hard map selection zones setup																															
	for (int col = 0; col < LEVEL_SELECT_SIZE; col++)
  	{
  		for (int row = 0; row < LEVEL_SELECT_SIZE; row++)
  		{
	  		map[col+27][row+9] = EASY;
	      	map[col+39][row+9] = MEDIUM;
	      	map[col+51][row+9] = HARD;
  		}
  	}

    // print the level selection map
	printBoard(map);


	//make the player
	Actor player(PLAYER_CHAR, 40, 7, 100, PLAYER_COLOUR);
	
	
	//Menu Instructions
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
  	
  	for (int col = 27; col < 54; col++)
  	{
  		for (int row = 9; row < 12; row++)
  		{
  			map[col][row] = 0;
		}
	}
  	cout << "1";
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
  	while(player_spawn_x <= 0 && player_spawn_y <= 0 || map[player_spawn_x][player_spawn_y] != 0)
  	{
  		player_spawn_x = rand() % 80;
  		player_spawn_y = rand() % 21;
	}
	
  	printBoard(map);
  	player.update_location(player_spawn_x - player.get_x(), player_spawn_y - player.get_y());

  
	// make the goose
	Actor monster(MONSTER_CHAR, goose_spawn_x, goose_spawn_y, 100, GOOSE_COLOUR);
    
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
		if (keyEntered == TK_SPACE)
      		testMap(player, map, win_info[0], win_info[1], win_info[2], win_info[3], win);
		//valid key press checks that the player is hitting one of the arrow keys or the escape or close key
		//otherwise if the player was hitting arbitrary keys the player would miss their turn and the goose would move
        if (keyEntered != TK_ESCAPE && keyEntered != TK_CLOSE && validKeyPress(keyEntered))
        {
            if(powerup == 0  || powerup == 3)//has to be before powerups so they activate for next turn
            {
            	movePlayer(keyEntered, player, map, win, powerup, uses);
			}
            	
            	
            else if(powerup == 1 && uses > 0)//jumping two squares
            {
            	for(int moves = 0; moves < 2; moves++)
				{
					
					movePlayer(keyEntered, player, map, win, powerup, uses);
					if(win)
					{
						moves = 2;
					}
				}
            	uses --;
			}
			
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
    	    
    	    
			
    	    if (uses == 0)
				powerup = 0;
	    	
	    	
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
