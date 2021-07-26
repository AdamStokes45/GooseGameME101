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

//set up the console.   Don't modify this line!
Console out;

int main()
{
    // Set up for using rand() to generate random numbers (can be deleted if random numbers not used)
    srand(time(NULL));

	//Set up the window.  Don't edit these two lines
    terminal_open();
  	terminal_set(SETUP_MESSAGE);

/*
    The code below provides a skeleton of the game play.  You will need to 
    write code for setting up the game board, and playing the game itself.
    You can modify the code given as needed.
    
    Call the functions that you have written in the game play file.
*/
    // declare the game board "map"
	int level_selected = 0;
	int goose_spawn_x = 0, goose_spawn_y = 0;
  int wall_amount = 0, wall_length = 0, wall_width = 0, wall_spawn_x = 0, wall_spawn_y = 0, wall_orientation = 0;
  int win_x = 0, win_y = 0, win_size = 0;
	bool win = 0;
	
	int map[MAP_X][MAP_Y] = {0};

/*
    Initialize locations in the game board to have game features.  This
    would include anything that is static and doesn't move like a wall.  Hard
    coding them like you see below is a poor way to code this.  What if you 
    have many features to add to the game board?  Should you use a loop?  Does
    it make sense to store this information in a file?  Should code be a
    function as well?
*/

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


	//make menu selector
	Actor starter(PLAYER_CHAR, 40, 2, 100, PLAYER_COLOUR);

	//select level
	int keyEntered = TK_A;

 while(keyEntered != TK_ESCAPE && keyEntered != TK_CLOSE && level_selected == 0)
	{
	    // get player key press
	    keyEntered = terminal_read();

        if (keyEntered != TK_ESCAPE && keyEntered != TK_CLOSE)
        {
            // move the player, you can modify this function
    	    moveStarter(keyEntered, starter, map,level_selected);	  
  		}
	}
  
  cout << "Level: " << level_selected;
	
	
	terminal_clear_area(0,0,MAP_X,MAP_Y);
	terminal_refresh();
  
	if (level_selected == 3) // easy map generation
  {
  	wall_amount = rand() % 3 + 1;
    wall_width = 1;
    for (int wall_num = 0; wall_num < wall_amount; wall_num++)
    {
    	wall_length = rand() % 8 + 3;
      wall_spawn_x = rand() % (80 - wall_length);
      wall_spawn_y = rand() % (21 - wall_length);
      wall_orientation = rand() % 2;
      
      if (wall_orientation == 0)
      {
        for (int col = 0; col < wall_length; col++)
        {
          map[wall_spawn_x + col][wall_spawn_y] = WALL;
        }
      }
      else
      {
        for (int row = 0; row < wall_length; row++)
        {
          map[wall_spawn_x][wall_spawn_y+row] = WALL;
        }
      }
      
    }
  }
	else if (level_selected == 4) // medium map generation
  {
    wall_amount = rand() % 8 + 3;
    int wall_width = 1;
    for(int wall_num = 0; wall_num < wall_amount; wall_num++)
    {
    	wall_length = rand() % 12 + 5;
      wall_spawn_x = rand() % (80 - wall_length);
      wall_spawn_y = rand() % (21 - wall_length);
      wall_orientation = rand() % 2;
      
      if(wall_orientation == 0)
      {
      
      for (int col = 0; col < wall_length; col++)
  		{
      	map[wall_spawn_x + col][wall_spawn_y] = WALL;
      }
  }
      else
      {
      for (int row = 0; row < wall_length; row++)
  		{
      	map[wall_spawn_x][wall_spawn_y + row] = WALL;
      }
      }
  		
  }
      
    }
  
  else // hard map generation
  {
  	wall_amount = rand() % 11 + 10;
    /*
    for (int wall_num = 0; wall_num < wall_amount; wall_num++)
    {
    	wall_length = rand() % 8 + 3;
      wall_spawn_x = rand();
      wall_spawn_y = rand();
    }
    */
  }
  
  printBoard(map);

//do something based on the level selected from previous while loop

  // make the player
	Actor player(PLAYER_CHAR, 10,10, 100, PLAYER_COLOUR);
  
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
