/*  KonaneUtility.h
    Interface for utility functions used in Konane

    Project URL: http://brick.cs.uchicago.edu/Courses/CMSC-16200/2012/pmwiki/pmwiki.php/Student/KonaneAI

    Andrew MacKie-Mason
    University of Chicago, CMSC 16200 */

/*  This file contains the interface for various utility functions.
    Implementation can be found in KonaneUtility.cpp */

#include <vector>
#include "KonaneBoard.h"

#ifndef __KONANE_UTILITY_H_INCLUDED__
#define __KONANE_UTILITY_H_INCLUDED__

using namespace std;


//  Data type used to store lists of moves, which are sets of four integers.
#define MOVE_VECTOR vector<int>
#define MOVES_ARRAY vector< MOVE_VECTOR >


/*  display_board
    Parameters:     A pointer to a KonaneBoard object.
    Actions:        Prints an ASCII display of the board to cout. */
void display_board (KonaneBoard *);

/*  game_over
    Parameters:     board, a pointer to a KonaneBoard object.
    Return:         true if there are no more available moves on the board,
                    false otherwise. */
bool game_over (KonaneBoard *);



/*  generate_moves
    Parameters:     board, a pointer to a KonaneBoard object.
                    movebox, a pointer to the container object to put the moves into
    Actions:        Fills the movebox with the possible moves (represented in
                    <x1,y1,x2,y2> form) that can be taken by the player who's turn
                    it is in the game represented by board. */
void generate_moves (KonaneBoard *, MOVES_ARRAY *);



/*  generate_moves_piece
    Parameters:     board, a pointer to a KonaneBoard object
                    movebox, a pointer to the container object to put the moves into
                    (x,y), the location to derive moves for
    Action:         Finds all the moves originating at (x,y) and adds them to the end of
                    movebox. */
void generate_moves_piece (KonaneBoard *, MOVES_ARRAY *, int, int);



/*  move_vector_to_jump
    Parameters:     A pointer to a Konane Board object.
                    A pointer to a MOVE_VECTOR
    Actions:        Unboxes the components of the MOVE_VECTOR and runs the board's jump command. */
void move_vector_to_jump (KonaneBoard *, MOVE_VECTOR *);



/*  move_vector_to_unjump
    Parameters:     A pointer to a Konane Board object.
                    A pointer to a MOVE_VECTOR
    Actions:        Unboxes the components of the MOVE_VECTOR and runs the board's unjump command. */
void move_vector_to_unjump (KonaneBoard *, MOVE_VECTOR *);



/*  move
    Parameters:     board, a pointer to a KonaneBoard object.
    Actions:        Handles the process of requesting and running a move
                    given by the current player. Outsources getting a move
                    to get_move. */
void move (KonaneBoard *);



/*  get_move
    Parameters:     A pointer to a KonaneBoard (the game board)
                    A pointer to a MOVE_VECTOR (storage for the result)
    Return:         Stores the supplied move in the move_vector.
    Notes:          Will call get_move_AI if the current player is an AI. */
void get_move (KonaneBoard *, MOVE_VECTOR *);



/*  get_move_AI
    Parameters:     A pointer to a KonaneBoard (the game board)
                    A pointer to a move_vector (storage for the result)
    Return:         Stores the supplied move in the move_vector */
void get_move_AI (KonaneBoard *, MOVE_VECTOR *);

#endif
