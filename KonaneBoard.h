/*  KonaneBoard.h
    Interface for game logic for Konane

    Project URL: http://brick.cs.uchicago.edu/Courses/CMSC-16200/2012/pmwiki/pmwiki.php/Student/KonaneAI

    Andrew MacKie-Mason
    University of Chicago, CMSC 16200 */

/*  This file contains the KonaneBoard class interface. Implementation is found
    in KonaneBoard.cpp */

/*  Objects of the KonaneBoard class contain all of the information
    needed to play a game of Konane.
        board:      A grid of values representing the state of the board.
                    The first coordinate is read across, the second coordinate
                    is read down (see below). 0 represents empty, 1
                    represents black, 2 represents white.

        turn:       This stores the current player's turn. 1 represents black,
                    2 represents white.

        AI:         A two entry array of integers, representing whether players
                    1 (black) and 2 (white) are AIs, respectively. 1 = AI, 0 = human.

        KonaneBoard:Constructor. Sets up the board with alternating black and white
                    pieces, starting with black at (0,0). Black goes first, 

        remove:     Given two integers, this function removes the piece at that
                    place. It can be called from outside the class to initialize
                    the board, and will be used in the implementation of jump.

        jump:       Given four integers, this function jumps the piece at the
                    location indicated by the first two to the location
                    indicated at the second two, removing intermediate
                    pieces. If the move is illegal, no changes are made.
                    If it's successful, the turn indicator is also changed.

        check_turn: Simple access function for the turn variable. */


#ifndef __KONANE_BOARD_H_INCLUDED__
#define __KONANE_BOARD_H_INCLUDED__


/*  Usage statements for class functions below.

    KonaneBoard
    Parameters:     int, int, representing whether the players are AI
                    With no parameters, it assumes neither player is an AI
    Results:        Sets up board with alternating black and white pieces, starting
                    with black at (0,0).

    remove
    Parameters:     int, int, representing an (x,y) coordinate on the board.
    Results:        The piece at the specified location is removed.

    jump
    Parameters:     int, int, int, int, representing two sets of coordinates.
    Results:        The piece at the first position is moved to the second position,
                    if it is a legal move. Jumped pieces are removed. If successful,
                    the turn variable is changed.

    unjump
    Parameters:     int, int, int, int, representing two sets of coordinates.
    Results:        Inverse function to jump.

    check_turn
    Parameters:     none
    Return:         turn

    check_AI
    Parameters:     none
    Return:         true if the current player is an AI, false otherwise

    check_val
    Parameters:     int, int, representing an (x,y) coordinate on the board.
    Return:         Safe access to the board array. Returns -1 if passed an invalid
                    position. */


class KonaneBoard
{
    private:
        int board[6][6];
        int turn;           // 1 = black, 2 = white
        int AI[2];          // 1 = AI, 0 = human (position 0 = black...)
    public:
        KonaneBoard (int, int);
        KonaneBoard ();

        void remove (int,int);
        void jump (int,int,int,int);
        void unjump (int,int,int,int);
        void flip_turn ();

        int check_turn ();
        bool check_AI ();
        int check_val (int, int);
};


#endif


