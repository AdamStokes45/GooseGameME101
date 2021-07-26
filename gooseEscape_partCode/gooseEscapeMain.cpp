//MAIN
//gooseEscapeMain.cpp

#include <BearLibTerminal.h>
#include <cmath>
#include <ctime>
#include <iostream>  // Debugging tip:  You can still use cout to put debug messages on the regular console window
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

	int level_selected = 0;
	int goose_spawn_x = 0, goose_spawn_y = 0, player_spawn_x = -1, player_spawn_y = -1;
	bool win = 0;
	
	int map[MAP_X][MAP_Y] = {0};


	//easy medium hard maps delcared below																																
	for (int col = 0; col < LEVEL_SELECT_SIZE; col++)
  	{
  		for (int row = 0; row < LEVEL_SELECT_SIZE; row++)
  		{
	  		map[col+27][row+9] = EASY;
	      	map[col+39][row+9] = MEDIUM;
	      	map[col+51][row+9] = HARD;
  		}
  	}

    // Call the function to print the game board
	printBoard(map);


	//make the player
	Actor player(PLAYER_CHAR, 40, 8, 100, PLAYER_COLOUR);

	//select level
	int keyEntered = TK_A;

 	while(keyEntered != TK_ESCAPE && keyEntered != TK_CLOSE && level_selected == 0)
	{
	    // get player key press
	    keyEntered = terminal_read();

        if (keyEntered != TK_ESCAPE && keyEntered != TK_CLOSE)
        {
            // move the player, you can modify this function
    	    moveStarter(keyEntered, player, map, level_selected);	  
  		}
	}
  
  	cout << "Level: " << level_selected;
	
	terminal_clear_area(0, 0, MAP_X, MAP_Y);
	terminal_refresh();
  	
  	for (int col = 0; col < MAP_X; col++)
  	{
  		for (int row = 0; row < MAP_Y; row++)
  		{
  			map[col][row] = 0;
		}
	}
  	
	if (level_selected == 3) // easy map generation
  	{
  		generateRandomArea(1, 3, 1, 1, 3, 8, map, WALL);
  		generateRandomArea(1, 1, 2, 3, 2, 3, map, WINNER);
  	}
	else if (level_selected == 4) // medium map generation
  	{
  		generateRandomArea(3, 8, 1, 1, 5, 12, map, WALL);
  		generateRandomArea(1, 1, 2, 2, 2, 2, map, WINNER);
    }
  
  	else // hard map generation
  	{
  		generateRandomArea(10, 11, 2, 3, 10, 11, map, WALL);
  		generateRandomArea(1, 1, 1, 1, 1, 1, map, WINNER);
  	}
  	while(player_spawn_x == -1 && player_spawn_y == -1 && map[player_spawn_x][player_spawn_y] != 0)
  	{
  		player_spawn_x = rand() % 80;
  		player_spawn_y = rand() % 21;
	}
  	printBoard(map);
  	player.spawn(player_spawn_x, player_spawn_y);

  
	// make the goose
	Actor monster(MONSTER_CHAR, goose_spawn_x, goose_spawn_y, 100, GOOSE_COLOUR);
    
	// Printing the instructions in the console window
    out.writeLine("Escape the Goose! " + monster.get_location_string());
	out.writeLine("Use the arrow keys to move");
	out.writeLine("If the goose catches you, you lose!");
	out.writeLine("Be careful! Sometimes the goose can jump through walls!");


      // can be any valid value that is not ESCAPE or CLOSE
    
    while(keyEntered != TK_ESCAPE && keyEntered != TK_CLOSE 
                    && !captured(player,monster) && !win)
	{
	    // get player key press
	    keyEntered = terminal_read();

        if (keyEntered != TK_ESCAPE && keyEntered != TK_CLOSE)
        {
            // move the player, you can modify this function
    	    movePlayer(keyEntered, player, map,win);
	    
    	    // call the goose's chase function
    	    moveGoose(player, monster, map);
    	    // call other functions to do stuff?
    	    
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
