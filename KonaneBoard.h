/*  KonaneBoard.h
    Interface for game logic for Konane

    Project URL: http://brick.cs.uchicago.edu/Courses/CMSC-16200/2012/pmwiki/pmwiki.php/Student/KonaneAI

    Andrew MacKie-Mason
    University of Chicago, CMSC 16200 */

/*  This file contains the KonaneBoard class interface. Implementation is found
    in KonaneBoard.cpp */


#include <string>
#include <vector>

#ifndef __KONANE_BOARD_H_INCLUDED__
#define __KONANE_BOARD_H_INCLUDED__

#define MOVE_VECTOR vector<int>
#define MOVES_ARRAY vector< MOVE_VECTOR >

using namespace std;


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

class KonaneBoard; // Forward declaration

class KonaneAI
{
    private:
        string AIName;
        int timeframe;
        double (*static_score_eval)(KonaneBoard *board, int player, int depth);
        double (*end_game_eval)(KonaneBoard *board, int player, int depth);
    public:
        KonaneAI (double(*static_score)(KonaneBoard *board, int player, int depth),
                  double(*end_game)(KonaneBoard *board, int player, int depth),
                  int timef, string n);
        KonaneAI (double(*static_score)(KonaneBoard *board, int player, int depth),
                  double(*end_game)(KonaneBoard *board, int player, int depth),
                  int timeframe);
        KonaneAI ();

        double recurse_score (KonaneBoard *board, int player, time_t stop_time, int depth);
        void best_move (KonaneBoard *board, MOVE_VECTOR *movevec);
};


//  Some different functions that can be used for a specific AI's static_score_eval
double static_score_simple (KonaneBoard *board, int player, int depth);
double static_score_new (KonaneBoard *board, int player, int depth);

//  Some different functions that can be used for a specific AI's end_game_eval
double end_game_simple (KonaneBoard *board, int player, int depth);
double end_game_depthadj (KonaneBoard *board, int player, int depth);


class KonaneBoard
{
    private:
        int board[6][6];
        int turn;
        int AI[2];
        KonaneAI *AIPointers[2];
        vector<string> names;

    public:
        KonaneBoard (KonaneAI *AI_1, KonaneAI *AI_2, string p1name, string p2name);
        KonaneBoard (int which_AI, KonaneAI *AIPnt, string p1name, string p2name);
        KonaneBoard (string p1name, string p2name);
        KonaneBoard ();

        void remove (int,int);
        void jump (int,int,int,int);
        void unjump (int,int,int,int);
        void flip_turn ();  // Use with CAUTION in hypothetical explorations of the game tree

        void get_move_AI (MOVE_VECTOR *movevec);

        int check_turn ();
        bool check_AI ();
        int check_val (int, int);
        string get_name (int player);
};





#endif


