/*  KonaneBoard.cpp
    Implementation of game logic for Konane

    Project URL: http://brick.cs.uchicago.edu/Courses/CMSC-16200/2012/pmwiki/pmwiki.php/Student/KonaneAI

    Andrew MacKie-Mason
    University of Chicago, CMSC 16200 */

/*  This file contains the KonaneBoard class implementation. The interface
    and detailed documentation comments are found in KonaneBoard.h */

#include "KonaneBoard.h"
#include <cmath>

using namespace std;

/*  KonaneBoard class constructors
    Declared in KonaneBoard.h

    With parameters passed, sets up board with appropriate AI designations.
    Without parameters, assumes all human players. */
KonaneBoard::KonaneBoard (int AI_1, int AI_2)
{
    AI[0] = AI_1; AI[1] = AI_2;
    turn = 1;

    int x;
    for (x = 0; x <= 5; ++x)
    {
        int y;
        for (y = 0; y <= 5; ++y)
        {
            board[x][y] = (x+y) % 2 + 1;    // Black (1) goes on even parity spots
        }
    }
}

KonaneBoard::KonaneBoard ()
{
    AI[0] = 0; AI[1] = 0;
    turn = 1;

    int x;
    for (x = 0; x <= 5; ++x)
    {
        int y;
        for (y = 0; y <= 5; ++y)
        {
            board[x][y] = (x+y) % 2 + 1;    // Black (1) goes on even parity spots
        }
    }
}

/*  remove
    
    Removes the piece that is at (x,y) */
void KonaneBoard::remove (int x,int y)
{
    board[x][y] = 0;
}

/*  jump

    If legal, jumps the piece at (x1,y1) to (x2,y2), removing intermediate pieces. */
void KonaneBoard::jump (int x1, int y1, int x2, int y2)
{
    if (board[x1][y1] == turn)
    {
        // Try to do something about the massive code duplication here
        if (x1==x2 and (y1-y2)%2==0 and y1 != y2)
        {
            bool valid_move = true;
            int dist = fabs(y1-y2);
            int dir = (y2-y1) / dist;       // make sure jumps in right direction

            int i;
            for (i = 1; i <= dist/2; ++i)
            {
                int y_temp = y1+2*i*dir;
                if (not ((check_val(x1, y_temp-dir) == 3-turn) and (check_val(x1, y_temp) == 0)))
                {
                    valid_move = false;
                }
            }
            if (valid_move)
            {
                for (i = 1; i <= dist/2; ++i)
                {
                    int y_temp = y1+2*i*dir;
                    board[x1][y_temp-2*dir] = 0;
                    board[x1][y_temp-dir] = 0;
                    board[x1][y_temp] = turn;
                }
                turn = 3-turn;
            }
        }
        else if (y1==y2 and (x1-x2)%2==0 and x1 != x2)
        {
            bool valid_move = true;
            int dist = fabs(x1-x2);
            int dir = (x2-x1) / dist;       // make sure jumps in right direction

            int i;
            for (i = 1; i <= dist/2; ++i)
            {
                int x_temp = x1+2*i*dir;
                if (not ((check_val(x_temp-dir,y1) == 3-turn) and (check_val(x_temp, y1) == 0)))
                {
                    valid_move = false;
                }
            }
            if (valid_move)
            {
                for (i = 1; i <= dist/2; ++i)
                {
                    int x_temp = x1+2*i*dir;
                    board[x_temp-2*dir][y1] = 0;
                    board[x_temp-dir][y1] = 0;
                    board[x_temp][y1] = turn;
                }
                turn = 3-turn;
            }
            
        }
    }      
}

/*  unjump

    Inverse function to jump, above.
    FIX: try to do something about code duplication with jump. */
void KonaneBoard::unjump (int x1, int y1, int x2, int y2)
{
    if (check_val(x1,y1) == 0)
    {
        // Try to do something about the massive code duplication here
        if (x1==x2 and (y1-y2)%2==0 and y1 != y2)
        {
            bool valid_move = true;
            int dist = fabs(y1-y2);
            int dir = (y2-y1) / dist;       // make sure jumps in right direction

            int i;
            for (i = 1; i <= dist/2 - 1; ++i)
            {
                int y_temp = y1+2*i*dir;
                if (not ((check_val(x1, y_temp-dir) == 0) and (check_val(x1, y_temp) == 0)))
                {
                    valid_move = false;
                }
            }

            if (not ((check_val(x1,y1+dist*dir-dir) == 0) and (check_val(x1,y1+dist*dir) == 3-turn)))
            {
                valid_move = false;
            }

            if (valid_move)
            {
                for (i = dist/2; i >= 1; --i)
                {
                    int y_temp = y1+2*i*dir;
                    board[x1][y_temp-2*dir] = 3-turn;
                    board[x1][y_temp-dir] = turn;
                    board[x1][y_temp] = 0;
                }
                turn = 3-turn;
            }
        }
        else if (y1==y2 and (x1-x2)%2==0 and x1 != x2)
        {
            bool valid_move = true;
            int dist = fabs(x1-x2);
            int dir = (x2-x1) / dist;       // make sure jumps in right direction

            int i;
            for (i = 1; i <= dist/2 - 1; ++i)
            {
                int x_temp = x1+2*i*dir;
                if (not ((check_val(x_temp-dir,y1) == 0) and (check_val(x_temp, y1) == 0)))
                {
                    valid_move = false;
                }
            }

            if (not ((check_val(x1+dist*dir-dir,y1) == 0) and (check_val(x1+dist*dir,y1) == 3-turn)))
            {
                valid_move = false;
            }

            if (valid_move)
            {
                for (i = dist/2; i >= 1; --i)
                {
                    int x_temp = x1+2*i*dir;
                    board[x_temp-2*dir][y1] = 3-turn;
                    board[x_temp-dir][y1] = turn;
                    board[x_temp][y1] = 0;
                }
                turn = 3-turn;
            }
            
        }
    }      
}

/*  flip_turn
    Takes no actions, but flips the turn.
    MAKE SURE TO ALWAYS UNDO. */
void KonaneBoard::flip_turn ()
{
    turn = 3 - turn;
}

/*  check_turn
    Access function for turn */
int KonaneBoard::check_turn ()
{
    return turn;
}

/*  check_AI
    Returns true if the current player is an AI, false otherwise. */
bool KonaneBoard::check_AI ()
{
    return (AI[turn-1] == 1);
}

/*  check_val

    Safe access to the board array. Returns -1 if passed an invalid position. */
int KonaneBoard::check_val (int x, int y)
{
    if (0 <= x and x <= 5 and 0 <= y and y <= 5)    // Safe access
    {
        return board[x][y];
    }
    else
    {
        return -1;
    }
}
